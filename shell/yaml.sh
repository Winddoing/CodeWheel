#!/bin/bash

#yaml格式
# aa: 123
# AA: xxxxxx
# 
# TST:
#   bb: 567
#   BB: 00


function parse_yaml()
{
    local s='[[:space:]]*' w='[a-zA-Z0-9_]*' fs=$(echo @|tr @ '\034')
    sed -ne "s|^\($s\)\($w\)$s:$s\"\(.*\)\"$s\$|\1$fs\2$fs\3|p" \
        -e "s|^\($s\)\($w\)$s:$s\(.*\)$s\$|\1$fs\2$fs\3|p"  $1 |
    awk -F$fs '{
        indent = length($1)/2;
        vname[indent] = $2;
        for (i in vname) {if (i > indent) {delete vname[i]}}
        if (length($3) > 0) {
         vn=""; for (i=0; i<indent; i++) {vn=(vn)(vname[i])("_")}
         printf("%s%s=\"%s\"\n", vn, $2, $3);
        }
    }'
}

yaml_file=$1

echo "yaml_file: $yaml_file"

eval $(parse_yaml $yaml_file)

echo "aa: $aa"
echo "AA: $AA"
echo "TST_bb: $TST_bb"
echo "TST_BB: $TST_BB"
