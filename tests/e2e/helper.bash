#!/usr/bin/bash

: ${CC:=cc}
BIN="$BATS_TEST_DIRNAME/../../main"
INPUTS="$BATS_TEST_DIRNAME/files"
EXPECTED="$BATS_TEST_DIRNAME/expected"

compare_output() {
  set -ao pipefail

  if ! [ -z "$UPDATE" ]; then
    $BIN $INPUTS/$1.c > $EXPECTED/output/$1.c
  fi

  diff -w <($BIN $INPUTS/$1.c) $EXPECTED/output/$1.c
}

compare_optimized() {
  set -ao pipefail

  if ! [ -z "$UPDATE" ]; then
    $BIN -O $INPUTS/$1.c > $EXPECTED/optimized/$1.c
  fi

  diff -w <($BIN -O $INPUTS/$1.c) $EXPECTED/optimized/$1.c
}

compare_ast() {
  set -ao pipefail

  if ! [ -z "$UPDATE" ]; then
    $BIN -a $INPUTS/$1.c > $EXPECTED/ast/$1.ast
  fi

  diff <($BIN -a $INPUTS/$1.c) $EXPECTED/ast/$1.ast
}

compare_exec() {
  set -ao pipefail
  $CC $INPUTS/$1.c -o $BATS_TMPDIR/$1-original -lm
  $BIN    $INPUTS/$1.c | $CC -x c -o $BATS_TMPDIR/$1-generated - -lmpc -lmpfr
  $BIN -O $INPUTS/$1.c | $CC -x c -o $BATS_TMPDIR/$1-optimized - -lmpc -lmpfr
  diff -w <($BATS_TMPDIR/$1-original) <($BATS_TMPDIR/$1-generated)
  diff -w <($BATS_TMPDIR/$1-original) <($BATS_TMPDIR/$1-optimized)
}
