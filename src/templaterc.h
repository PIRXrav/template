/*
 * templaterc.h
 *
 *  Created on: 10/06/2020
 *      Author: pirx
 */

#ifndef _templaterc_H_
#define _templaterc_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void RC_Init(char *name, char *templaterc);
char *RC_Get(char *key);
void RC_Print(void);
void RC_Free(void);

#endif /* _templaterc_H_ */
