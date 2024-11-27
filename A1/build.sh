#!/bin/sh

CFLAGS="-O2 -Wall -Wextra"
COMPILER=x86_64-w64-mingw32-gcc 

$COMPILER runner.c -o runner.exe
$COMPILER source.c -o source.exe
$COMPILER output.c -o output.exe

