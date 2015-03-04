#!/bin/bash
set -e

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd ${DIR}/..

cmake -H. -DCMAKE_BUILD_TYPE=Debug -Beclipse-project -G"Eclipse CDT4 - Unix Makefiles"
