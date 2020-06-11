/*
 * args.c
 *
 *  Created on: 10/06/2020
 *      Author: pirx
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "args.h"
#include <argp.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Internal function declaration
 ******************************************************************************/

static error_t parse_opt(int key, char *arg, struct argp_state *state);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Program documentation. */
static char doc[] = "A template creator";

/* A description of the arguments we accept. */
static char args_doc[] = "FILE";

/* The options we understand. */
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output", 0},
    {"type", 't', "NAMEFILE", 0, "The template type (ex : -t class)", 0},
    {0}};

static struct arguments arguments;

/*******************************************************************************
 * Public function
 ******************************************************************************/

struct arguments *ARGS_Parse(int argc, char **argv) {

  arguments.pathfile = NULL;
  arguments.namefile = NULL;
  arguments.format = NULL;

  arguments.verbose = 0;
  arguments.type = "0";
  static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  return &arguments;
}

/*******************************************************************************
 * Internal function
 ******************************************************************************/

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
  case 'v':
    arguments->verbose = 1;
    break;
  case 't': {
    arguments->type = arg;
    break;
  }
  case ARGP_KEY_ARG: {
    if (state->arg_num == 1)
      /* Too many arguments. */
      argp_usage(state);
    size_t buff_size = strlen(arg) + 1;
    char *buffer = calloc(buff_size, sizeof(char));

    // find last .
    int i_point;
    for (i_point = buff_size - 1; i_point >= 0; i_point--) {
      if (arg[i_point] == '.')
        break;
    }

    int i_name; // '/'
    for (i_name = i_point - 1; i_name >= 0; i_name--) {
      if (arg[i_name] == '/')
        break;
      buffer[i_name] = arg[i_name];
    }

    arguments->pathfile = arg;
    arguments->format = arg + i_point + 1;
    arguments->namefile =
        strcpy(malloc(i_point - i_name + 1), buffer + i_name + 1); // secure
    free(buffer);
    break;
  }
  case ARGP_KEY_END:
    if (state->arg_num < 1)
      /* Not enough arguments. */
      argp_usage(state);
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

void ARGS_Free(void) { free(arguments.namefile); }
