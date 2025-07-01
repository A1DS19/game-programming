#!/usr/bin/env python
import sys

if len(sys.argv) < 2:
    sys.exit("Usage: cc_args.py <compile_commands.json>")

with open(sys.argv[1], 'r') as f:
    print(f.read())
