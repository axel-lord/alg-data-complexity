#include "sized_array.h"

/* sorts given sized sized_array in-place
 * Parameters:
 * Arr, array to sort
 * Returns:
 * 1, if sorting went as planned
 * 0, if something went wrong
 * Note:
 * if 0 is returned the array may be a bit
 * scrambled but all elements should still
 * be in the array
 */
int mergesort_array(sized_array * Arr);
