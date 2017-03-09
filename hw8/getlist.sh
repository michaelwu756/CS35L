#!/bin/sh

while read line
do
    echo "ldd /usr/bin/$line"
    ldd /usr/bin/$line
done </dev/stdin