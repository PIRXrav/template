/*
 * args.h
 *
 *  Created on: 10/06/2020
 *      Author: pirx
 */

#ifndef _args_H_
#define _args_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *namefile; /* NAME */
  int verbose;
  char *format;
  long type;
};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

struct arguments *ARGS_Parse(int argc, char **argv);

#endif /* _args_H_ */