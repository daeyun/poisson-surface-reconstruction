#!/bin/bash
set -e

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd ${DIR}

sudo gcc-config 1
. /etc/profile

cmake -H. -Bbuild
make -j13 -Cbuild

echo "Build successful"
sudo gcc-config 2
. /etc/profile

sudo cp build/lib/libpoisson_recon_lib.so /usr/lib/
