#include "args.h"
#include "editor.h"
#include "path.h"
#include "templaterc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  struct arguments *args = ARGS_Parse(argc, argv);
  if (args->verbose) {
    printf("namefile %s\n", args->namefile);
    printf("pathfile %s\n", args->pathfile);
    printf("format   %s\n", args->format);
    printf("verbose  %s\n", args->verbose ? "True" : "False");
    printf("template %s.%s\n", args->type, args->format);
  }

  PATH_Init();
  if (args->verbose) {
    printf("PATH : %s\n", template_path);
    printf("rc.sh: %s\n", templaterc);
  }

  RC_Init(args->namefile, templaterc);
  if (args->verbose) {
    RC_Print();
  }

  EDITOR_Create(args->type, args->format, args->pathfile);

  if (args->verbose) {
    printf("DONE\n");
  }
  RC_Free();
  ARGS_Free();
  return 0;
}
