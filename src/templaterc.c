/*
 * templaterc.c
 *
 *  Created on: 10/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#define _DEFAULT_SOURCE
#include "path.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

struct tup_str {
  char *key;
  char *dat;
  struct tup_str *next;
};

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

void free_rec(struct tup_str *ts);

/*******************************************************************************
 * Variables
 ******************************************************************************/

struct tup_str *head = NULL;

/*******************************************************************************
 * Public function
 ******************************************************************************/

void RC_Init(char *name, char *templaterc) {
  FILE *frc = fopen(templaterc, "r");
  if (!frc) {
    perror("No template configuration file\n");
    exit(1);
  }
  const size_t buff_size = 255;
  char buff[buff_size];
  size_t i_buff = 0;
  char ch;
  head = malloc(sizeof(struct tup_str));
  head->key = strcpy(malloc(5), "NAME");
  head->dat = strcpy(malloc(strlen(name) + 1), name);
  head->next = NULL;

  char buff_key[1024];
  char buff_dat[1024];
  char cmd[2048];

  while ((ch = fgetc(frc)) != EOF) {
    if (ch == '\n') {
      buff[i_buff] = '\0';
      i_buff = 0;
      // printf("%s\n", buff);
      if (sscanf(buff, " %[a-zA-Z0-9_] = %[^\n]", buff_key, buff_dat)) { // ok
        // printf("KEY : %s \nDAT : %s \n", buff_key, buff_dat);
        // Apply bash code on temp var
        char buff_out[1024];
        size_t i_buff_out = 0;
        sprintf(cmd, "temp=%s && echo $temp", buff_dat);
        FILE *fp = popen(cmd, "r");
        while ((ch = fgetc(fp)) != EOF) {
          if (ch == '\n')
            break;
          buff_out[i_buff_out++] = ch;
        }
        buff_out[i_buff_out++] = '\0';
        fclose(fp);
        // printf("OUT : %s\n", buff_out);
        // Create new node
        struct tup_str *new_head = calloc(1, sizeof(struct tup_str));
        assert(new_head);
        new_head->next = head;
        head = new_head;
        // add data
        head->key = strcpy(malloc(strlen(buff_key) + 1), buff_key);
        head->dat = strcpy(malloc(strlen(buff_out) + 1), buff_out);
      }
    } else {

      assert(i_buff < buff_size);
      buff[i_buff++] = ch;
    }
  }
  fclose(frc);
}

char *RC_Get(char *key) {
  for (struct tup_str *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(key, cur->key) == 0) {
      return cur->dat;
    }
  }
  return "UNDEFINED";
}

void RC_Print(void) {
  for (struct tup_str *cur = head; cur != NULL; cur = cur->next) {
    printf("%s --> %s\n", cur->key, cur->dat);
  }
}

void RC_Free(void) { free_rec(head); }

/*******************************************************************************
 * Internal function
 ******************************************************************************/

void free_rec(struct tup_str *ts) {
  if (ts->next)
    free_rec(ts->next);
  free(ts->key);
  free(ts->dat);
  free(ts);
}
