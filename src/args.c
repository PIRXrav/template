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
static char args_doc[] = "NAME";

/* The options we understand. */
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output", 0},
    {"format", 'f', "NAME", 0, "The desired language (ex : -f py)", 0},
    {"type", 't', "NUMBER", 0, "The template type (ex : -t 2)", 0},
    {0}};

/*******************************************************************************
 * Public function
 ******************************************************************************/

struct arguments *ARGS_Parse(int argc, char **argv) {
  static struct arguments arguments;
  /* Default values. */
  arguments.namefile = NULL;
  arguments.verbose = 0;
  arguments.format = "c";
  arguments.type = 0;
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
  case 'f':
    arguments->format = arg;
    break;
  case 't': {
    char *endPtr;
    arguments->type = strtol(arg, &endPtr, 10);
    if (endPtr == arg)
      argp_usage(state);
    break;
  }
  case ARGP_KEY_ARG:
    if (state->arg_num == 1)
      /* Too many arguments. */
      argp_usage(state);
    arguments->namefile = arg;
    break;
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
