/** file: cbc_lib.c
*
* Implements the functions used in cbc_lib.h
*
* @author: Gabe Rippel | gwr3294@rit.edu
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cbc_lib.h"

#define BITS_IN_BLOCK (sizeof( long ) * 8)
#define INPUT_SIZE (sizeof(long) * 24)

static block64 key = 0x1234DeadBeefCafe; // each hex digit is 4 bits
static const block64 INITIALIZATION_VECTOR = 0x0L; // initial IV value

void b64_string(char * string, block64 text) {
    strcopy(data, (char *) &txt, sizeof(long));
}

/// performs a bit barrel roll right to left by count bits.
/// @param block the block64 subjected to roll.
/// @param count the number of bits to be cycled
/// @return the changed block data
block64 roll_right(block64 block, size_t count) {
    return (block >> count) | (block << (BITS_IN_BLOCK - count));
}

/// performs the barrel roll opposite roll_right.
/// @param block the block64 subjected to roll.
/// @param count the number of bits to be cycled
/// @return the changed block data
block64 roll_left(block64 block, size_t count) {
    return (block << count) | (block >> (BITS_IN_BLOCK - count));    
}

int encode(const char * destpath) {
    return 1;
}

int decode(const char * sourcepath) {
    return 1;
}


