#!/bin/bash
set -auxeo pipefail

rm -rf ./tests/e2e/tmp ./a.out
mkdir ./tests/e2e/tmp
: ${CC:=cc}

$CC ./tests/e2e/files/$1.c -o ./tests/e2e/tmp/$1 -lm

./main ./tests/e2e/files/$1.c

diff -w <(./main ./tests/e2e/files/$1.c) ./tests/e2e/expected/$1.c
./main tests/e2e/files/$1.c | gcc -x c - -lmpc -lmpfr
diff -w <(./tests/e2e/tmp/$1) <(./a.out)

rm -rf ./tests/e2e/tmp ./a.out
