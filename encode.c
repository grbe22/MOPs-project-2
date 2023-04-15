/** File: encode.c
 *
 * Gets input from stdout, encrypts, and outputs cyphertext to a file.
 *
 *@author: Gabriel Rippel, gwr3294@rit.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbc_lib.h"

/// handles input & passes it to cbc_lib.c
/// @param argc: number of commands passed through command line.
/// @param argv: string list of commands passed through command line.
int main (int argc, char * argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: encode to-file-name  # from standard input\n");
        return EXIT_FAILURE;
    }
    // if encode returns a 0, as in it successfully encodes, returns success. else, returns failure.
    if (encode(argv[1]) == 0) {
        fprintf(stderr, "ok\n");
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "FAILED\n");
        return EXIT_FAILURE;
    }
}
