#!/usr/bin/python3
# coding=utf-8

# Base Python File (asound-enum-params.py)
# Created: Mon 05 Mar 2018 10:21:19 PM CET
# Version: 1.0
#
# This Python script was developped by François-Xavier Thomas.
# You are free to copy, adapt or modify it.
#
# (ɔ) François-Xavier Thomas <fx.thomas@gmail.com>


"""Enumerates known and unknown Intel HDA parameters for a given node
This script needs the `hda-verb` tool found in the `alsa-tools` package in
Archlinux.
"""

import subprocess
import argparse
import sys
import re

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("device", help="Audio device (e.g. /dev/snd/hwC0D0)")
parser.add_argument("node_id", help="HDA Node ID")
parser.add_argument("--verb", help="HDA Verb to use (default is 'PARAMETERS')", default="PARAMETERS")  # noqa
parser.add_argument("--bits", help="Number of bits in the HDA payload", type=int, default=8)
args = parser.parse_args()

values = []
for payload in range(2**args.bits):
    output = subprocess.check_output([
        "sudo",
        "hda-verb",
        args.device,
        args.node_id,
        args.verb,
        "0x%x" % payload
    ])
    for line in output.split(b"\n"):
        rel = re.match(rb"value = 0x([0-9a-f]+)", line)
        if not rel:
            continue
        values.append(int(rel.group(1), 16))

for i, v in enumerate(values):
    sys.stdout.write("%08x" % v)
    if (i+1) % 16 == 0:
        sys.stdout.write("\n")
    else:
        sys.stdout.write(" ")
