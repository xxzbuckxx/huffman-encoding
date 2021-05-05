#include "bm.h"
#include "hamming.h"
#include "nibble.h"
#include "translator.h"

#include <stdbool.h>
#include <stdio.h> // Printing
#include <stdlib.h>
#include <unistd.h> // For getopt()

#define OPTIONS "hvi:o:"

#define FILE_NOT_FOUND "File not found ;(\n"

#define HELP "how to use here"

int main(int argc, char **argv) {

    /* if (argc < 2) { */
    /*     printf(HELP); */
    /*     return 1; // error */
    /* } */

    FILE *file_in = stdin;
    FILE *file_out = stdout;

    bool verbose = false;

    // Parse

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': printf(HELP); break;
        case 'v': verbose = true; break;
        case 'i':
            if ((file_in = fopen(optarg, "r")) == NULL) {
                printf(FILE_NOT_FOUND);
                return 1; // error
            }
            break;
        case 'o':
            if ((file_out = fopen(optarg, "w")) == NULL) {
                printf(FILE_NOT_FOUND);
                return 1; // error
            }
            break;
        default: return 1; // error
        }
    }

    // Initalize
    BitMatrix *G = bm_create_encode();
    int buffer = 0x00;

    while ((buffer = fgetc(file_in)) != EOF) {
        uint8_t lower = lower_nibble(buffer);
        uint8_t encoded_lower = encode(G, lower);

        /* printf("encoded lower %c is %d\n\n", encoded_lower, encoded_lower); */
        uint8_t upper = upper_nibble((uint8_t) buffer);
        uint8_t encoded_upper = encode(G, upper);
        fputc(encoded_upper, file_out);
        fputc(encoded_lower, file_out);
        // now inherit file permision
    }

    /* BitMatrix *Ht = bm_create_decode(); */

    // while data still in file:
    // Read a byte using fgetc()
    // Generate Hamming(8,4) for both upper and lower nubble using ham_encode()
    //      write to file with fputc()
    //      Lower first then upper
    //

    bm_delete(&G);
    fclose(file_in);
    fclose(file_out);
    return 0;
}
