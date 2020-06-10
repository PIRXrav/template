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
void EDITOR_Create(size_t type, char *format, char *name) {
  char inname[1024];
  assert(snprintf(inname, 1024, "%s/%ld.%s", template_path, type, format));
  char outname[1024];
  assert(snprintf(outname, 1024, "./%s.%s", name, format));
  /*
  printf("INPUT  : %s\n", inname);
  printf("OUTPUT : %s\n", outname);
*/
  FILE *fin = fopen(inname, "r");
  if (!fin) {
    fprintf(stderr, "No template %s\n", inname);
    exit(1);
  }
  // Test out file
  FILE *fout = fopen(outname, "r");
  if (fout) { // Existing
    fclose(fout);
    fprintf(stderr, "Existing file %s\n", outname);
    exit(1);
  }
  fout = fopen(outname, "w");
  assert(fout);

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
