#!/usr/bin/env bats

load helper

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
