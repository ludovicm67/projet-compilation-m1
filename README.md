# C2MP

See <https://gliech.pages.unistra.fr/compilation/>

## Usage

```
Usage: c2mp [-hvOa] [-p precision] [-r rounding] [-o file] SOURCE

Compiling C to C with transformations of some arithmetic
computations so that these computations are carried out
in floating point of arbitrary precision, thanks to the use
of a dedicated library

  SOURCE is a file (`-' for stdin) to compile
Options:
  -o    Specifies the output (stdout by default)
  -v    Verbose output (repeatable)
  -O    Run optimizations
  -a    Print out the AST
  -p    Set a default precision
  -r    Set a default rounding
  -h    Display this help and exit
```

Sample input file:

```c
#include <stdio.h>
#include <mpfr.h>
#include <math.h>

#define NMAX 42

int main() {
  int pc = 2;
  double resultat;

  #pragma MPFR precision(128) rounding(MPFR_RNDZ)
  resultat = -(sqrt(-NMAX*(8*pc-4*pow(NMAX,3)-4*pow(NMAX,2)-NMAX-8))-2*pow(NMAX,2)-NMAX)/(2*NMAX);

  printf("==> %f\n", resultat);

  return 0;
}
```

## Building

You will need a C compiler (tested against GCC and clang), `make`, `flex` and `bison` for building.
[`bats`](https://github.com/bats-core/bats-core) and the MPC/MPFR library headers are needed for running the test suite (see `tests/main.c` and `tests/e2e/`).
`doxygen` and `dot` are required for building the documentation.

- `make` for building the binary
- `make doc` for running Doxygen
- `make clean` for cleaning all products
- `make release` for building a binary with optimizations enabled (`clean` first)
- `make test` for building the unit-test binary and run the unit and end-to-end tests
- `make cover` for building binaries with coverage enabled and running the test suite (`clean` first)

## Features

 - A lot of the C grammar is implemented in the AST, but it does not mean it generates code. Running C2MP with `-a` let's you see the tree.
 - Without optimizations, a lot of temporary variables are generated. With them enabled, dead code is removed.
 - The basic process looks like this:
   1. The parser yields an AST (`lexer.l`, `parser.y`)
   2. The AST is then consumed to generate quads and populate the symbol table (`ast.c`, `statement.c`).
      During this phase, the type of each symbol is checked (e.g. `x = y < z` means `x` is a `bool`, while `y` and `z` both need to be numeric values).
   3. If enabled, it runs the optimizations on the quads (`optim.c`)
     - First, basic arithmetic optimizations are done (`x = y * 1` yields `x = y` ; `x = y + 0` yields `x = y`)
     - Assignments are applied (when the compiler sees `x = y`, it aliases `x` to `y`)
     - Identical symbols are then merged
     - A kind of dependency graph is generated for the symbols
     - Dead code is eliminated by walking through the dependency graph starting from output symbols
   4. The different parts of the code are printed
     - The `init` section, where all the temporary variables are initialized
     - The `assign` section, where external variables and constants are set
     - The `operations` section, where each quad yields code
     - The `clear` section, where symbols are output and variables cleared
 - Booleans are kinda supported (but comparisons are not yet implemented)
 - Control statements (`if`, loops, `return`, `break`, `continue`) are not supported yet by the quad generator
 - Multiple levels of logging (`-v` option)

## Tests

A few unit tests are defined. They essentially cover operations on the AST, on statements, on quads and the code generation.

The generation of quads from the AST is not yet covered by the tests.

End to end tests are in the `tests/e2e/` directory.
You'll find in here examples of files that work with C2MP.

 - `tests/e2e/files/%.c` is the input file
 - `tests/e2e/expected/output/%.c` is the generated file
 - `tests/e2e/expected/optimized/%.c` is the generated file with optimizations enabled
 - `tests/e2e/expected/ast/%.ast` is the generated AST

Each test file is done so it can be compiled without C2MP transforming it first.
Each test file is compiled and run in its 3 forms (original, transformed, optimized) and the output is checked.

This allowed us to squash a lot of bugs and ensure optimizations yield the same results.

Tests are run with GitLab CI on each commit with both GCC and `clang`.


## Documentation

The documentation is generated by Doxygen and automatically published to <https://gliech.pages.unistra.fr/compilation/> by GitLab CI.

## Code

Code is hosted on GitLab: <https://git.unistra.fr/gliech/compilation>
