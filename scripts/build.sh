#!/bin/bash
set -e

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd ${DIR}/..

#sudo gcc-config 1
#. /etc/profile

rm -f *.mexa64

cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
make -j13 -Cbuild

echo "Build successful"
#sudo gcc-config 2
#. /etc/profile
