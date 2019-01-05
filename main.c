#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void parse(FILE* source);
int yyparse(void);

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

FILE* f_open(char* fname) {
  FILE* fp = fopen(fname, "r");
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

  FILE* src = stdin;  // STDIN is the source by default
  FILE* dst = stdout; // STDOUT is the destination by default

  // No option is required
  while ((option = getopt(argc, argv, "+:o:h")) != -1) {
    switch(option){
      case 'o':
        dst = f_open(optarg);
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
    src = f_open(argv[0]);
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

  (void) src;
  (void) dst;

  parse(src);
  /*
  yyin = src;
  yyparse();
  */

  f_close(src);
  f_close(dst);

  return EXIT_SUCCESS;
}
