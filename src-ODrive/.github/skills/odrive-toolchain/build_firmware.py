#!/usr/bin/env python3
"""
Build ODrive firmware for specified board configuration.

Usage:
    python build_firmware.py board-v3.6-56V
    python build_firmware.py board-v3.5-24V --clean
    python build_firmware.py --list-configs

Requirements:
    - ARM GCC toolchain installed
    - Tup build system installed
    - Python virtual environment (optional but recommended)

Environment Variables (Windows):
    TUP_PATH: Path to tup.exe (default: searches PATH)
    ARM_TOOLCHAIN_PATH: Path to ARM GCC bin folder
"""

import argparse
import os
import platform
import shutil
import subprocess
import sys
from pathlib import Path

# Available board configurations (maps to CONFIG_BOARD_VERSION in tup.config)
BOARD_CONFIGS = {
    'board-v3.6-56V': 'ODrive v3.6 (56V) - Current production',
    'board-v3.5-48V': 'ODrive v3.5 (48V) - Legacy',
    'board-v3.5-24V': 'ODrive v3.5 (24V) - Legacy',
}

# Map board config names to tup.config values
BOARD_CONFIG_MAP = {
    'board-v3.6-56V': 'v3.6-56V',
    'board-v3.5-48V': 'v3.5-48V',
    'board-v3.5-24V': 'v3.5-24V',
}

def find_firmware_dir():
    """Find the Firmware directory relative to script location."""
    script_dir = Path(__file__).parent.resolve()
    repo_root = script_dir.parent.parent.parent  # odrive-toolchain -> skills -> .github -> repo
    firmware_dir = repo_root / 'Firmware'
    
    if not firmware_dir.exists():
        print(f"❌ Error: Firmware directory not found at {firmware_dir}")
        print(f"   Script location: {script_dir}")
        print(f"   Repository root: {repo_root}")
        sys.exit(1)
    
    return firmware_dir

def find_tools():
    """Find required build tools (tup, ARM GCC)."""
    tools = {}
    
    # Find tup
    tup_path = os.environ.get('TUP_PATH')
    if tup_path and Path(tup_path).exists():
        tools['tup'] = tup_path
    else:
        tools['tup'] = shutil.which('tup') or shutil.which('tup.exe')
    
    if not tools['tup']:
        print("❌ Error: tup build tool not found")
        print("   Please install tup or set TUP_PATH environment variable")
        print("   Download: http://gittup.org/tup/")
        sys.exit(1)
    
    # Find ARM GCC
    arm_gcc = shutil.which('arm-none-eabi-gcc') or shutil.which('arm-none-eabi-gcc.exe')
    if not arm_gcc:
        print("❌ Error: ARM GCC toolchain not found")
        print("   Please install ARM GCC toolchain and add to PATH")
        print("   Download: https://developer.arm.com/downloads/-/gnu-rm")
        sys.exit(1)
    
    tools['arm_gcc'] = arm_gcc
    tools['arm_gcc_dir'] = str(Path(arm_gcc).parent)
    
    return tools

def check_venv():
    """Check if running in virtual environment."""
    in_venv = hasattr(sys, 'real_prefix') or (
        hasattr(sys, 'base_prefix') and sys.base_prefix != sys.prefix
    )
    if in_venv:
        print(f"✓ Virtual environment active: {sys.prefix}")
    return in_venv

def list_configurations():
    """List all available board configurations."""
    print("\n📋 Available Board Configurations:\n")
    for config, description in BOARD_CONFIGS.items():
        board_version = BOARD_CONFIG_MAP[config]
        print(f"  • {config:20} - {description}")
        print(f"    CONFIG_BOARD_VERSION={board_version}")
    print()

def update_tup_config(firmware_dir, board_config):
    """Update tup.config with the specified board version."""
    tup_config = firmware_dir / 'tup.config'
    tup_config_default = firmware_dir / 'tup.config.default'
    
    # Read default config as template
    if not tup_config_default.exists():
        print(f"❌ Error: {tup_config_default} not found")
        sys.exit(1)
    
    with open(tup_config_default, 'r') as f:
        config_content = f.read()
    
    # Update board version
    board_version = BOARD_CONFIG_MAP[board_config]
    config_lines = []
    for line in config_content.split('\n'):
        if line.strip().startswith('#CONFIG_BOARD_VERSION') or line.strip().startswith('CONFIG_BOARD_VERSION'):
            config_lines.append(f'CONFIG_BOARD_VERSION={board_version}')
        else:
            config_lines.append(line)
    
    # Write updated config
    with open(tup_config, 'w') as f:
        f.write('\n'.join(config_lines))
    
    print(f"✓ Updated tup.config: CONFIG_BOARD_VERSION={board_version}")

def clean_build(firmware_dir):
    """Clean the build and autogen directories."""
    print("🧹 Cleaning build artifacts...")
    
    build_dir = firmware_dir / 'build'
    autogen_dir = firmware_dir / 'autogen'
    
    removed = []
    if build_dir.exists():
        shutil.rmtree(build_dir, ignore_errors=True)
        removed.append('build/')
    
    if autogen_dir.exists():
        shutil.rmtree(autogen_dir, ignore_errors=True)
        removed.append('autogen/')
    
    # Recreate autogen directory
    autogen_dir.mkdir(exist_ok=True)
    
    if removed:
        print(f"✅ Cleaned: {', '.join(removed)}\n")
    else:
        print("✅ Nothing to clean\n")

def generate_version(firmware_dir, tools_dir):
    """Generate version.c file required by build system."""
    print("📝 Generating version.c...")
    
    autogen_dir = firmware_dir / 'autogen'
    autogen_dir.mkdir(exist_ok=True)
    
    version_script = tools_dir / 'tools' / 'odrive' / 'version.py'
    version_output = autogen_dir / 'version.c'
    
    if not version_script.exists():
        print(f"❌ Error: version.py not found at {version_script}")
        return False
    
    try:
        result = subprocess.run(
            [sys.executable, str(version_script), '--output', str(version_output)],
            cwd=firmware_dir,
            capture_output=True,
            text=True,
            check=True
        )
        
        # Extract version from output
        version_info = result.stdout.strip()
        print(f"✓ {version_info}")
        return True
        
    except subprocess.CalledProcessError as e:
        print(f"❌ Failed to generate version.c: {e}")
        if e.stderr:
            print(e.stderr)
        return False

def build_firmware(firmware_dir, config, tools):
    """Build firmware for specified configuration using tup."""
    print(f"\n🔨 Building firmware for {config}...")
    print(f"📂 Working directory: {firmware_dir}")
    print(f"🔧 Build tool: {tools['tup']}")
    print(f"🔧 ARM GCC: {tools['arm_gcc']}\n")
    
    # Update tup.config with board selection
    update_tup_config(firmware_dir, config)
    
    # Generate version.c
    repo_root = firmware_dir.parent
    if not generate_version(firmware_dir, repo_root):
        return False
    
    # Run tup build
    try:
        # Set up environment to ensure ARM GCC is in PATH
        env = os.environ.copy()
        if 'PATH' not in env:
            env['PATH'] = tools['arm_gcc_dir']
        elif tools['arm_gcc_dir'] not in env['PATH']:
            env['PATH'] = tools['arm_gcc_dir'] + os.pathsep + env['PATH']
        
        print("Building with tup...")
        result = subprocess.run(
            [tools['tup'], '--no-environ-check'],
            cwd=firmware_dir,
            capture_output=True,
            text=True,
            check=True,
            env=env
        )
        
        # Parse output for size information
        output_lines = result.stdout.split('\n')
        for i, line in enumerate(output_lines):
            if 'arm-none-eabi-size' in line and i + 1 < len(output_lines):
                # Found size command, next lines have the data
                size_header = output_lines[i + 1] if i + 1 < len(output_lines) else ""
                size_data = output_lines[i + 2] if i + 2 < len(output_lines) else ""
                
                if 'text' in size_header and size_data:
                    print("\n✅ Build completed successfully!")
                    print(f"\n📊 Binary Size:")
                    print(f"   {size_header}")
                    print(f"   {size_data}")
                    break
        else:
            print("\n✅ Build completed successfully!")
        
        # Check for output files
        build_dir = firmware_dir / 'build'
        if build_dir.exists():
            elf_file = build_dir / 'ODriveFirmware.elf'
            bin_file = build_dir / 'ODriveFirmware.bin'
            hex_file = build_dir / 'ODriveFirmware.hex'
            
            print(f"\n📦 Output Files:")
            if elf_file.exists():
                size_mb = elf_file.stat().st_size / (1024 * 1024)
                print(f"   • {elf_file.name} ({size_mb:.2f} MB)")
            if bin_file.exists():
                size_kb = bin_file.stat().st_size / 1024
                print(f"   • {bin_file.name} ({size_kb:.1f} KB)")
            if hex_file.exists():
                size_kb = hex_file.stat().st_size / 1024
                print(f"   • {hex_file.name} ({size_kb:.1f} KB)")
            
            print(f"\n📂 Build directory: {build_dir}")
        
        return True
        
    except subprocess.CalledProcessError as e:
        print(f"\n❌ Build failed!")
        print(f"\nError output:")
        if e.stdout:
            print(e.stdout)
        if e.stderr:
            print(e.stderr)
        return False

def main():
    parser = argparse.ArgumentParser(
        description='Build ODrive firmware for specified board configuration',
        epilog='Example: python build_firmware.py board-v3.6-56V --clean'
    )
    parser.add_argument(
        'config',
        nargs='?',
        choices=list(BOARD_CONFIGS.keys()),
        help='Board configuration to build'
    )
    parser.add_argument(
        '--clean',
        action='store_true',
        help='Clean build directory before building'
    )
    parser.add_argument(
        '--list-configs',
        action='store_true',
        help='List all available configurations'
    )
    parser.add_argument(
        '--check-tools',
        action='store_true',
        help='Check if required build tools are available'
    )
    
    args = parser.parse_args()
    
    # Handle list configurations
    if args.list_configs:
        list_configurations()
        sys.exit(0)
    
    # Find and verify tools
    print("🔍 Checking build environment...")
    check_venv()
    tools = find_tools()
    print(f"✓ Tup: {tools['tup']}")
    print(f"✓ ARM GCC: {tools['arm_gcc']}")
    print()
    
    if args.check_tools:
        print("✅ All required tools found!")
        sys.exit(0)
    
    # Require board configuration for build
    if not args.config:
        print("❌ Error: Board configuration required")
        list_configurations()
        sys.exit(1)
    
    # Find firmware directory
    firmware_dir = find_firmware_dir()
    print(f"✓ Firmware directory: {firmware_dir}\n")
    
    # Clean if requested
    if args.clean:
        clean_build(firmware_dir)
    
    # Build firmware
    success = build_firmware(firmware_dir, args.config, tools)
    
    if success:
        print("\n🎉 Build process completed successfully!")
    else:
        print("\n💥 Build process failed!")
    
    sys.exit(0 if success else 1)

if __name__ == '__main__':
    main()
