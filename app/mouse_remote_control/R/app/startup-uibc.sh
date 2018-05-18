#!/bin/sh

CONSOLE=/dev/ttyS000

echo "Startup vx uibc ..." > ${CONSOLE}
/bin/vx_uibc &

