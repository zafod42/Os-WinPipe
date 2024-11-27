#!/bin/sh

wine64 bin/runner.exe

echo 
printf "\033[33m FINAL RESULTS \033[0m\n"
echo "=== SOURCE ==="
echo
cat file.txt
echo

echo "=== RESULT === (snake_cased)"
echo

cat output.txt
echo

