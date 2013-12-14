#!/bin/sh
make clean
make exploit_1
./exploit_1
echo "badfile created; running retlib..."
./retlib