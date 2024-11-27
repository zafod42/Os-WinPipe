#!/bin/sh

CFLAGS="-O2 -Wall -Wextra"
COMPILER=x86_64-w64-mingw32-gcc 
BUILD_DIR=bin/
SRC_DIR=src/

$COMPILER $SRC_DIR/runner.c -o $BUILD_DIR/runner.exe
$COMPILER $SRC_DIR/source.c -o $BUILD_DIR/source.exe
$COMPILER $SRC_DIR/output.c -o $BUILD_DIR/output.exe

