#!/bin/bash
set -e

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd ${DIR}/..

if hash gcc-config 2>/dev/null; then
    sudo gcc-config 4.7.3
    . /etc/profile
fi

rm -f *.mexa64

cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug
make -j13 -Cbuild

echo "Build successful"

if hash gcc-config 2>/dev/null; then
    sudo gcc-config 4.9.2
    . /etc/profile
fi
