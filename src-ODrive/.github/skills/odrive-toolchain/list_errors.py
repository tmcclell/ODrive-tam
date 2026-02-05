#!/usr/bin/env python3
"""
List error codes from ODrive interface YAML.

Usage:
    python list_errors.py
    python list_errors.py --filter encoder
    python list_errors.py --format table
"""

import argparse
import re
import sys
from pathlib import Path


def find_repo_root():
    """Find the repository root directory relative to script location."""
    # Script is at: .github/skills/odrive-toolchain/list_errors.py
    # Repo root is 3 levels up: odrive-toolchain -> skills -> .github -> repo
    script_dir = Path(__file__).resolve().parent
    return script_dir.parent.parent.parent


def parse_errors_from_yaml(yaml_path: Path) -> list:
    """Parse error enums from odrive-interface.yaml."""
    errors = []
    
    if not yaml_path.exists():
        print(f"❌ File not found: {yaml_path}")
        return errors
    
    with open(yaml_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # Find error flags sections (ODrive format: flags: section under error: attribute)
    # Pattern matches flag names like: CONTROL_ITERATION_MISSED:, DC_BUS_UNDER_VOLTAGE:, etc.
    # These are uppercase names with underscores followed by a colon at start of line (with indent)
    flag_pattern = re.compile(r'^\s{10,14}([A-Z][A-Z0-9_]+):\s*$', re.MULTILINE)
    
    for match in flag_pattern.finditer(content):
        flag_name = match.group(1)
        # Skip common non-error flags
        if flag_name in ('NONE', 'True', 'False'):
            continue
        errors.append({
            'name': flag_name,
            'value': 'flag'
        })
    
    # Also find error enums using old format (ERROR_SOMETHING: 0x0001)
    error_pattern = re.compile(r'^\s*(ERROR_\w+)\s*:\s*(0x[0-9a-fA-F]+|\d+)', re.MULTILINE)
    
    for match in error_pattern.finditer(content):
        error_name = match.group(1)
        error_value = match.group(2)
        if not any(e['name'] == error_name for e in errors):
            errors.append({
                'name': error_name,
                'value': error_value
            })
    
    return errors


def format_table(errors: list) -> str:
    """Format errors as a table."""
    if not errors:
        return "No errors found"
    
    max_name_len = max(len(e['name']) for e in errors)
    
    lines = []
    lines.append(f"{'Error Name':<{max_name_len}}  Value")
    lines.append('-' * (max_name_len + 12))
    
    for error in errors:
        lines.append(f"{error['name']:<{max_name_len}}  {error['value']}")
    
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description='List ODrive error codes')
    parser.add_argument('--filter', '-f', help='Filter errors by keyword')
    parser.add_argument('--format', choices=['text', 'table'], default='text',
                        help='Output format')
    
    args = parser.parse_args()
    
    repo_root = find_repo_root()
    yaml_path = repo_root / 'Firmware' / 'odrive-interface.yaml'
    
    print(f"📋 Reading: {yaml_path}\n")
    
    errors = parse_errors_from_yaml(yaml_path)
    
    if args.filter:
        filter_lower = args.filter.lower()
        errors = [e for e in errors if filter_lower in e['name'].lower()]
        print(f"🔍 Filtered by: {args.filter}\n")
    
    if not errors:
        print("❌ No errors found")
        return 1
    
    print(f"✅ Found {len(errors)} error code(s):\n")
    
    if args.format == 'table':
        print(format_table(errors))
    else:
        for error in errors:
            print(f"  • {error['name']}: {error['value']}")
    
    return 0


if __name__ == '__main__':
    sys.exit(main())
