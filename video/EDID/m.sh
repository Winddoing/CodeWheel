##########################################################
# File Name		: m.sh
# Author		: winddoing
# Created Time	: 2019年03月19日 星期二 11时53分45秒
# Description	:
##########################################################
#!/bin/bash

if [ "$1" == "clean" ]; then
    echo "xxxxxxxxxxxxxx"
    rm bin_to_8bit_hex build_edid conver_edid edid.h edid.hex hex_to_bin new_edid.bin
    exit 1;
fi


gcc bin_to_8bit_hex.c -o bin_to_8bit_hex
gcc build_edid.c -o build_edid
gcc conver_edid.c -o conver_edid
gcc hex_to_bin.c -o hex_to_bin


bin_to_array --one edid.bin edid.h

./bin_to_8bit_hex edid.bin edid.hex

./hex_to_bin edid.hex new_edid.bin

diff edid.bin new_edid.bin


