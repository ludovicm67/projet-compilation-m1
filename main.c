#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "statement.h"
#include "gencode.h"

stmt_t *parse(FILE* source);
int yyparse(void);

FILE *f_dst;

void usage (char * const command) {
    printf("Usage: %s [-o file] SOURCE\n\n", command);
    printf("Compiling C to C with transformations of some arithmetic\n"
           "computations so that these computations are carried out \n"
           "in floating point of arbitrary precision, thanks to the use\n"
           "of a dedicated library\n\n");
    printf("  SOURCE is a file (or stdin) to compile\n");
    printf("Options:\n"
           "  -o   Specifies the output (stdout or a file)\n"
           "  -h   Display this help and exit\n");
    printf("\n");

    exit(EXIT_FAILURE);
}

FILE* f_open(char* fname, char* options) {
  FILE* fp = fopen(fname, options);
  if (!fp) {
    perror("open");
    abort();
  }
  return fp;
}

void f_close(FILE* fp) {
  fclose(fp);
  if (ferror(fp)) {
    perror("close");
    abort();
  }
}


int main (int argc, char* argv[]) {
  char* prog_name = argv[0];

  int option;
  int i = 0;

  FILE* f_src = stdin; // STDIN is the source by default
  f_dst = stdout;      // STDOUT is the destination by default

  // No option is required
  while ((option = getopt(argc, argv, "+:o:h")) != -1) {
    switch(option){
      case 'o':
        f_dst = f_open(optarg, "w");
        break;
      case 'h':
        usage(prog_name);
        break;
      case '?':
        fprintf(stderr, "Unknown option: %c\n\n", optopt);
        usage(prog_name);
        break;
      case ':':
        fprintf(stderr, "Option -%c takes an argument\n", optopt);
        usage(prog_name);
        break;
      default :
        abort();
    }
  }

  argc -= optind;
	argv += optind;

  if (argc == 1) {
    printf("source : %s\n", argv[0]);
    f_src = f_open(argv[0], "r");
  }

  if (argc > 1) {
    printf("\nToo much argument(s): ");
    while (i < argc) {
      printf("%s ", argv[i]);
      i++;
    }
    printf("\n\n");
    usage(prog_name);
  }

  gencode_args_t args;
  args.file = stdout;
  args.lib = 0;
  args.precision = 128;
  args.rounding = "MPCMACHIN";
  args.file = f_dst;

  stmt_t *result = NULL;

  while ((result = parse(f_src)) != NULL) {
    //stmt_display(result);

    op_list_t *ops = NULL;
    symbol_t *table = NULL;
    stmt_gen_quad(result, &table, &ops);

    gencode_init(&args, table);
    gencode_assign(&args, table);
    gencode_operations(&args, ops);
    gencode_clear(&args, table);

    stmt_delete(result);
    quad_list_delete(ops);
    symbol_delete(table);
  }

  f_close(f_src);
  f_close(f_dst);

  return EXIT_SUCCESS;
}
