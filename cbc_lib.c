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
#define BYTES_PER_BLOCK (sizeof(long))

static block64 key = 0x1234DeadBeefCafe; // each hex digit is 4 bits
static const block64 INITIALIZATION_VECTOR = 0x0L; // initial IV value

union Block64_string {
    char string[sizeof(long)];
    block64 block;
};

/// converts a byte64 element into a string.
/// string: an array of string that should be undefined.
/// text: the block64 being converted to a string.
void block64_to_string(block64 text, char * string) {
    
    /// clears data. this way, terminated strings will have empty bytes after termination byte.
    memset(string, '\0', sizeof(long) + 1);
    strncpy(string, (char *) &text, sizeof(long));
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


/// handles input and passes it to b_c_d. returns information as a string.
/// accepts cipher, the current block, count, the length of the block, vector, where it's stored, and key, the encryption key.
char * cbc_decrypt(block64 * cipher, int count, block64 * vector, block64 key) {
    union Block64_string * text;
    text = malloc((count * sizeof(long)) + 1);
    // scrubs any data stored in text.
    memset(text, '\0', count * sizeof(long) + 1);
    for (int i = 0; i < count; i ++) {
        text -> block = block_cipher_decrypt(cipher[i], key) ^ *vector;
        *vector = cipher[i];
    }
    return(text -> string);
}


/// takes the source file location of the encrypted text and processes it for cbc_decrypt.
/// passes the information in manageable chunks to cbc_decrypt.
/// returns failure if the file isn't found, success if it is. found.
int decode(const char * sourcepath) {
    FILE * file;
    if ((file = fopen(sourcepath, "rb")) == NULL) {
        return EXIT_FAILURE;
    }

    block64 * block;
    block = (block64 *) malloc(sizeof(block64));
    block64 vector = INITIALIZATION_VECTOR;
    int count = 0;

    while ((count = fread(block, 1, sizeof(block64), file)/sizeof(block64))) {
        char * decryption = cbc_decrypt(&block[0], count, &vector, key);
        printf("%s", decryption);
        free(decryption);
    }

    free(block);
    fclose(file);
    return EXIT_SUCCESS;
}


/// takes the text input from encode, the vector, and the key, and encrypts it.
/// text: the text provided by encode
/// vector: the vector location of the program.
/// key: the encryption key that encrypt uses.
block64 * cbc_encrypt(char * text, block64 * vector, block64 key) {
    // add sizeof(long) to handle escape characters.
    int count = 1+strlen(text)/sizeof(long);
    block64 * piece = (block64 *) malloc(count * sizeof(long));
    for (int i = 0; i < count; i ++) {
        // instantiates a buffer with null character filling.
        char buffer [sizeof(long) + 1] = {"\0"};
        strncpy(buffer, text + (i * sizeof(long)), sizeof(long));
        block64 * block = (block64 *) buffer;
        piece[i] = * block ^ * vector;
        * vector = block_cipher_encrypt(piece[i], key);
        piece[i] = * vector;
    }
    return(piece);
}


/// takes the string for the destination, user input, then packages & processes the information for cbc_encrypt.
/// takes a file destination as a parameter.
/// returns exit success if it successfully encrypts, exit failure if the file isn't found.
int encode (const char * destpath) {
    FILE * file;
    if ((file = fopen(destpath, "wb")) == NULL) {
        return EXIT_FAILURE;
    }
    char * text;
    text = (char *) malloc(INPUT_SIZE);
    block64 vector = INITIALIZATION_VECTOR;
    while (fgets(text, INPUT_SIZE, stdin) != NULL) {
        block64 * block = cbc_encrypt(text, &vector, key);
        fwrite(block, sizeof(long), 1+strlen(text)/8, file);
        free(block);
    }
    free(text);
    fclose(file);
    return EXIT_SUCCESS;
}
