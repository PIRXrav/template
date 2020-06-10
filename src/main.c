#include "args.h"
#include "editor.h"
#include "path.h"
#include "templaterc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {

  struct arguments *arguments = ARGS_Parse(argc, argv);
  if (arguments->verbose) {
    printf("namefile %s\n", arguments->namefile);
    printf("verbose  %s\n", arguments->verbose ? "True" : "False");
    printf("template %ld.%s\n", arguments->type, arguments->format);
  }

  PATH_Init();
  if (arguments->verbose) {
    printf("PATH : %s\n", template_path);
    printf("rc.sh: %s\n", templaterc);
  }

  RC_Init(arguments->namefile, templaterc);
  if (arguments->verbose) {
    RC_Print();
  }

  EDITOR_Create(arguments->type, arguments->format, arguments->namefile);

  if (arguments->verbose) {
    printf("DONE\n");
  }
  RC_Free();
  return 0;
}
