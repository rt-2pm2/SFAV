/*=================================================================
 * mxcreateuninitnumericmatrix.c 
 * Create an uninitialized mxArray, fill it with the contents 
 * of "data", and return it in plhs[0].
 *
 * Input:   none
 * Output:  mxArray
 *
 * Copyright 2015 The MathWorks, Inc.
 *	
 *=================================================================*/
#include "mex.h"

/* The mxArray in this example is 2x2 */
#define ROWS 2
#define COLUMNS 2
#define ELEMENTS 4

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
{
    double  *pointer;          /* pointer to real data in new array */
    mwSize index;
    const double data[] = {2.1, 3.4, 2.3, 2.45};  /* existing data */

    /* Check for proper number of arguments. */
    if ( nrhs != 0 ) {
        mexErrMsgIdAndTxt("MATLAB:mxcreateuninitnumericmatrix:rhs",
                "This function takes no input arguments.");
    } 

    /* Create an m-by-n mxArray. */
    plhs[0] = mxCreateUninitNumericMatrix(ROWS, COLUMNS, mxDOUBLE_CLASS, mxREAL);

    /* Copy existing data. */
    pointer = mxGetPr(plhs[0]);
    for ( index = 0; index < ELEMENTS; index++ ) {
        pointer[index] = data[index];
    }
    return;
}
