/** File: decode.c
 *
 * gets input from a file and outputs cyphertext to standard output.
 *
 * Author: Gabe Rippel gwr3294@rit.edu
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cbc_lib.h"

/// parses input to cbc_lib.c to decode.
/// @param argc: number of cmd line arguments
/// @param argv: the cmd line arguments.
/// returns EXIT_SUCCESS if successful, EXIT_FAILURE if failed.
int main(int argc, char * argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: decode file-name  # to standard input\n\n");
        return EXIT_FAILURE;
    }
    // same as in encode.
    if (decode(argv[1]) == 0) {
        fprintf(stderr, "ok\n");
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "FAILED\n");
        return EXIT_FAILURE;
    }
}
