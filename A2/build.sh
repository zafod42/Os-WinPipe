#!/bin/sh

CFLAGS="-O2 -Wall -Wextra"
COMPILER=x86_64-w64-mingw32-gcc 
BUILD_DIR=bin/
SRC_DIR=src/

SOURCES=`ls src`

for src in $SOURCES; do
    target=`echo $src | sed "s/\.c//"`
    $COMPILER $SRC_DIR/$src -o $BUILD_DIR/$target
done


