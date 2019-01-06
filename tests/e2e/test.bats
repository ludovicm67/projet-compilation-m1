#!/usr/bin/env bats

@test "test1" {
  run ./tests/e2e/test.sh test1
  [ "$status" -eq 0 ]
}

@test "test2" {
  run ./tests/e2e/test.sh test2
  [ "$status" -eq 0 ]
}

