#!/bin/bash

PWD=`pwd`

git ls-tree -r --name-only HEAD | while read filename; do
  echo -e "$(git log -1 --format="\e[1;33m [%an] \e[0m:%ae" -- $filename) \e[1;31m File \e[0m:$PWD/$filename"
  done
