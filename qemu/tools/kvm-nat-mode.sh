#!/bin/bash

if [[ $1 == add ]]; then
	iptables -t filter -I FORWARD -o virbr0 -j ACCEPT
elif [[ $1 == del ]]; then
	iptables -t filter -D FORWARD -o virbr0 -j ACCEPT
else
        echo "usage: $0 add|del"
fi
