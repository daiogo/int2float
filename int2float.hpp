/*
	int2float
	by Diogo Freitas
*/

#ifndef INT2FLOAT_H_
#define INT2FLOAT_H_

/*******************************************************************************
*	INCLUDES
*******************************************************************************/
#include <iostream>
#include <bitset>
#include <iomanip>
#include <cstring>
#include <assert.h>

/*******************************************************************************
*	DEFINITIONS
*******************************************************************************/
typedef union _float_union {
	float f;
	unsigned u;
} float_union;

/*******************************************************************************
*	FUNCTION DECLARATIONS
*******************************************************************************/
float int2float(int num);

#endif /* INT2FLOAT_H_ */