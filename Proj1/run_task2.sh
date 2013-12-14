#!/bin/sh
make clean
make exploit_2
./exploit_2
echo "badfile created; running retlib..."
./retlib