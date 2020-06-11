/*
 * editor.c
 *
 *  Created on: 10/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "editor.h"
#include "path.h"
#include "templaterc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define TEMPLATE_DELIMITOR '$'

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Public function
 ******************************************************************************/

// Parse type.file and gen outfile.file
void EDITOR_Create(char *type, char *format, char *outpath) {
  char *inpath =
      malloc(strlen(template_path) + strlen(type) + strlen(format) + 2 + 1);
  sprintf(inpath, "%s/%s.%s", template_path, type, format);

  // printf("INPUT PATH  : %s\n", inpath);
  // printf("OUTPUT PATH : %s\n", outpath);

  // Set in FILE*
  FILE *fin = fopen(inpath, "r");
  if (!fin) {
    fprintf(stderr, "No template %s\n", inpath);
    exit(1);
  }
  free(inpath);

  // Set out FILE*
  FILE *fout = fopen(outpath, "r");
  if (fout) { // Existing
    fclose(fout);
    fprintf(stderr, "Existing file %s\n", outpath);
    exit(1);
  }
  fout = fopen(outpath, "w");
  if (!fout) {
    fprintf(stderr, "Inpossible to create %s\n", outpath);
    exit(1);
  }

  // Cpy in >(w/RC) out
  const size_t buff_size = 255;
  char buff[buff_size];
  size_t i_buff = 0;
  char ch;
  int state = 0; // 0 : out $$; 1 : in $$
  while ((ch = fgetc(fin)) != EOF) {
    if (state == 0) { // out
      if (ch == TEMPLATE_DELIMITOR) {
        state = 1 - state;
      } else {
        fputc(ch, fout);
      }
    } else { // in
      if (ch == TEMPLATE_DELIMITOR) {
        state = 1 - state;
        buff[i_buff] = '\0';
        fputs(RC_Get(buff), fout);
        i_buff = 0;
      } else {
        assert(i_buff < buff_size);
        buff[i_buff++] = ch;
      }
    }
  }

  fclose(fin);
  fclose(fout);
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/
