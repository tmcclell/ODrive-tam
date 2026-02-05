#!/usr/bin/env python3
"""
Search for symbols in the ODrive codebase.

Usage:
    python search_symbol.py Controller
    python search_symbol.py "ERROR_" --regex
    python search_symbol.py Encoder --type class
"""

import argparse
import os
import re
import subprocess
import sys
from pathlib import Path


def find_repo_root():
    """Find the repository root directory relative to script location."""
    # Script is at: .github/skills/odrive-toolchain/search_symbol.py
    # Repo root is 3 levels up: odrive-toolchain -> skills -> .github -> repo
    script_dir = Path(__file__).resolve().parent
    return script_dir.parent.parent.parent


def search_with_git_grep(pattern: str, is_regex: bool = False) -> list:
    """Search using git grep (faster for git repos)."""
    repo_root = find_repo_root()
    
    try:
        cmd = ['git', 'grep', '-n', '--color=never']
        if is_regex:
            cmd.append('-E')
        cmd.append(pattern)
        
        result = subprocess.run(
            cmd,
            cwd=repo_root,
            capture_output=True,
            text=True
        )
        
        if result.returncode == 0:
            return result.stdout.strip().split('\n')
        return []
    except Exception:
        return []


def search_with_findstr(pattern: str, symbol_type: str = None) -> list:
    """Fallback search using file system."""
    repo_root = find_repo_root()
    results = []
    
    extensions = ['.cpp', '.hpp', '.c', '.h', '.py']
    exclude_dirs = {'build', '.git', 'node_modules', '__pycache__', '.tup'}
    
    # Adjust pattern based on type
    if symbol_type == 'class':
        patterns = [f'class\\s+{pattern}', f'struct\\s+{pattern}']
    elif symbol_type == 'function':
        patterns = [f'{pattern}\\s*\\(', f'def\\s+{pattern}']
    else:
        patterns = [pattern]
    
    for root, dirs, files in os.walk(repo_root):
        # Skip excluded directories
        dirs[:] = [d for d in dirs if d not in exclude_dirs]
        
        for file in files:
            if not any(file.endswith(ext) for ext in extensions):
                continue
            
            filepath = Path(root) / file
            try:
                with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                    for line_num, line in enumerate(f, 1):
                        for p in patterns:
                            if re.search(p, line):
                                rel_path = filepath.relative_to(repo_root)
                                results.append(f"{rel_path}:{line_num}:{line.strip()}")
                                break
            except Exception:
                continue
    
    return results


def main():
    parser = argparse.ArgumentParser(description='Search for symbols in the ODrive codebase')
    parser.add_argument('symbol', help='Symbol to search for')
    parser.add_argument('--regex', '-r', action='store_true', help='Treat symbol as regex')
    parser.add_argument('--type', '-t', choices=['class', 'function', 'variable'],
                        help='Filter by symbol type')
    
    args = parser.parse_args()
    
    print(f"🔍 Searching for: {args.symbol}")
    if args.type:
        print(f"   Type filter: {args.type}")
    print()
    
    # Try git grep first, then fallback
    if args.type:
        results = search_with_findstr(args.symbol, args.type)
    else:
        results = search_with_git_grep(args.symbol, args.regex)
        if not results:
            results = search_with_findstr(args.symbol)
    
    if not results or (len(results) == 1 and not results[0]):
        print("❌ No matches found")
        return 1
    
    print(f"✅ Found {len(results)} match(es):\n")
    for result in results[:50]:  # Limit output
        if result:
            print(f"  {result}")
    
    if len(results) > 50:
        print(f"\n  ... and {len(results) - 50} more matches")
    
    return 0


if __name__ == '__main__':
    sys.exit(main())
