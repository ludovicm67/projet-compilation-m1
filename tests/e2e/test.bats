#!/usr/bin/env bats

load helper

@test "Report syntax error" {
  run $BIN -o /dev/null $INPUTS/syntax-error.c
  [ "$status" -eq 2 ]
  [[ "$output" == *"syntax error"* ]]
}

@test "Report invalid operation" {
  run $BIN -o /dev/null $INPUTS/invalid-op.c
  [ "$status" -eq 1 ]
  [[ "$output" == *"Can't use a decimal symbol ("*") in a and op"* ]]
}

@test "Try different log levels" {
  run $BIN -o /dev/null $INPUTS/simple.c
  [ -z "$output" ] # No output

  run $BIN -vvo /dev/null $INPUTS/simple.c
  [[ "$output" == *"INFO"* ]]
  [[ "$output" != *"DEBUG"* ]]

  run $BIN -vvvo /dev/null $INPUTS/simple.c
  [[ "$output" == *"INFO"* ]]
  [[ "$output" == *"DEBUG"* ]]
}

@test "Should display usage" {
  run $BIN -h
  [[ "$output" == *"Usage"* ]]
  run $BIN -K # Invalid argument
  [[ "$output" == *"Unknown option: K"* ]]
  [[ "$output" == *"Usage"* ]]
  run $BIN -o # No output file specified
  [[ "$output" == *"Usage"* ]]
}

@test "'simple' AST" { compare_ast simple; }
@test "'simple' output" { compare_output simple; }
@test "'simple' optimized" { compare_optimized simple; }
@test "'simple' execution" { compare_exec simple; }

@test "'empty' AST" { compare_ast empty; }
@test "'empty' output" { compare_output empty; }
@test "'empty' optimized" { compare_optimized empty; }

@test "'constant-alias' AST" { compare_ast constant-alias; }
@test "'constant-alias' output" { compare_output constant-alias; }
@test "'constant-alias' optimized" { compare_optimized constant-alias; }
@test "'constant-alias' execution" { compare_exec constant-alias; }

@test "'multiple-variables' AST" { compare_ast multiple-variables; }
@test "'multiple-variables' output" { compare_output multiple-variables; }
@test "'multiple-variables' optimized" { compare_optimized multiple-variables; }
@test "'multiple-variables' execution" { compare_exec multiple-variables; }

@test "'single-long-expr' AST" { compare_ast single-long-expr; }
@test "'single-long-expr' output" { compare_output single-long-expr; }
@test "'single-long-expr' optimized" { compare_optimized single-long-expr; }
@test "'single-long-expr' execution" { compare_exec single-long-expr; }

@test "'control-statements' AST" { compare_ast control-statements; }
@test "'control-statements' output" { skip; compare_output control-statements; }
@test "'control-statements' optimized" { skip; compare_optimized control-statements; }
@test "'control-statements' execution" { skip; compare_exec control-statements; }
