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

#define BITS_IN_BLOCK (sizeof(long) * 8)
#define INPUT_SIZE (sizeof(long) * 24)

static block64 key = 0x1234DeadBeefCafe; // each hex digit is 4 bits
static const block64 INITIALIZATION_VECTOR = 0x0L; // initial IV value

union block64_string {
    char string[sizeof(long)];
    block64 block;
}

/// converts a byte64 element into a string.
/// string: an array of string that should be undefined.
/// text: the block64 being converted to a string.
///void b64_string(char * string, block64 text) {
    /// clears data. this way, terminated strings will have empty bytes after termination byte.
    ///memset(string, '\0', sizeof(long) + 1);
    ///strcopy(string, (char *) &txt, sizeof(long));
///}

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

/// converts plaintext blocks to encrypted blocks.
/// repeated four times, block is shifted 10 to the left then xor'ed with key.
/// this ensures an unreadable value unless you have the key value.
block64 block_cipher_encrypt(block64 block, block64 key) {
    for (int i = 0; i < 4; i++) {
        block = roll_left(block, 10);
        block = block ^ key;
    } 
    return block;
}

/// mirrors the implementation of b_c_e.
/// xor can be undone by recalling xor on the same bits, then it's shifted right.
/// repeat four times to match b_c_d.
block64 block_cipher_decrypt(block64 block, block64 key) {
    for (int i = 0; i < 4; i++) {
        block = block ^ key;
        block = roll_right(block, 10);
    }
    return block;
}

int encode(const char * destpath) {
    return 1;
}

b64_string * cbc_decrypt(block64 * cipher, int count, block64 key, block64 * vector) {
    b64_string * text = (char *) malloc(count + 1);
    // scrubs any data stored in text.
    memset(text, '\0', count + 1);
    for (int i = 0; i < count; i ++) {
        text.block = block_cipher_decrypt(cipher[i], key) ^ vector;
        vector = cipher[i]
    }
    return(text);
}

int decode(const char * sourcepath) {
    FILE * file;
    if ((fp = fopen(sourcepath, "rb")) == NULL) {
        return EXIT_FAILURE;
    }

    block64 * block;
    block = (block64 *) malloc(sizeof(block64));
    block64 vector = INITIALIZATION_VECTOR;
    int c = 0;

    while (c = fread(block, sizeof(block64), file)) {
        b64_string * decryption = cbc_decrypt(&block[0], count, key, vector);
        printf("%s", decryption.string);
        free(decryption);
    }

    free(block);
    fclose(file);
    return EXIT_SUCCESS;
}
