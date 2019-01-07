#!/usr/bin/env bats

load helper

@test "test1" {
  compare_output test1
  compare_optimized test2
  compare_ast test2
  compare_exec test1
}

@test "test2" {
  compare_output test2
  compare_optimized test2
  compare_ast test2
  compare_exec test2
}

@test "test3" {
  compare_output test3
  compare_optimized test3
  compare_ast test3
  compare_exec test3
}
