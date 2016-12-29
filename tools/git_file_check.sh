#!/bin/sh
git ls-tree -r --name-only HEAD | while read filename; do
  echo "$(git log -1 --format="%an %ae" -- $filename) $filename"
  done
