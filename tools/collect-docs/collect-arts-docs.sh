#!/bin/bash
#
# Collects all ARTS documentation from the specified build directory
# in the current folder for publication on Github
#
# Author: Oliver Lemke  <oliver.lemke@uni-hamburg.de>

if [[ $# != 1 ]]; then
    echo "$0 PATH_TO_ARTS_BUILD_DIR"
    exit 1
fi

if [[ ! -d $1 ]]; then
    echo "$1 is not a directory"
    exit 1
fi

rm -rf docserver doxygen uguide pyarts
mkdir uguide

cp -r $1/python/doc/build pyarts
cp $1/doc/uguide/*.pdf uguide/
rm -rf pyarts/.doctrees
