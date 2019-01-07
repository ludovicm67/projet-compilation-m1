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
  [[ "$output" == *"Unknown option"* ]]
  [[ "$output" == *"Usage"* ]]
  run $BIN -o # No output file specified
  [[ "$output" == *"Usage"* ]]
}

@test "Try different rounding/precision args" {
  run $BIN <(echo "#pragma MPC 3;")
  echo "$output"
  [ "$status" -eq 0 ]
  diff -w <(echo "$output") - <<EOF

    // declaration of all variables that we will use
    mpc_t c2mp_temp0; mpc_init2(c2mp_temp0, 128);

    // assign values to some variables
    mpc_set_d(c2mp_temp0, 3.000000, MPC_RNDZZ);

    // free memory of all variables that we used
    mpc_clear(c2mp_temp0);
EOF

  run $BIN -r CUSTOM <(echo "#pragma MPC 3;")
  echo "$output"
  [ "$status" -eq 0 ]
  diff -w <(echo "$output") - <<EOF

    // declaration of all variables that we will use
    mpc_t c2mp_temp0; mpc_init2(c2mp_temp0, 128);

    // assign values to some variables
    mpc_set_d(c2mp_temp0, 3.000000, CUSTOM);

    // free memory of all variables that we used
    mpc_clear(c2mp_temp0);
EOF

  run $BIN -p 256 <(echo "#pragma MPC 3;")
  echo "$output"
  [ "$status" -eq 0 ]
  diff -w <(echo "$output") - <<EOF

    // declaration of all variables that we will use
    mpc_t c2mp_temp0; mpc_init2(c2mp_temp0, 256);

    // assign values to some variables
    mpc_set_d(c2mp_temp0, 3.000000, MPC_RNDZZ);

    // free memory of all variables that we used
    mpc_clear(c2mp_temp0);
EOF
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
