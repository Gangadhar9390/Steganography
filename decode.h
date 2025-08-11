#ifndef DECODE_H
#define DeCODE_H

#include <stdio.h>
#include "types.h" // User-defined data types

/*
 * Structure to store all information required for
 * decoding a secret file from a stego image.
 * This includes input file details, buffers, and
 * decoded file metadata.
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    char *magic_string;                     // Expected magic string to validate presence of hidden data
    char secret_fname[50];                  // Output filename for the decoded secret
    FILE *fptr_secret;                      // File pointer to store decoded secret file
    char extn_secret_file[MAX_FILE_SUFFIX]; // Extension of the secret file (e.g., .txt, .c)

    int secret_file_extn_size; // Length of secret file extension
    int size_secret_file;      // Size of the decoded secret file

    /* Stego Image Info */
    char *stego_image_fname; // Name of the stego image (input)
    FILE *fptr_stego_image;  // File pointer to the stego image

} DecodeInfo;

/* Function Prototypes for Decoding */

/* Identify the operation type (encode/decode) based on command-line arguments */
OperationType check_operation_type(char *argv[]);

/* Read and validate decoding arguments from command line */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Main function to perform the decoding process */
Status do_decoding(DecodeInfo *decInfo);

/* Decode the magic string from the stego image and verify it */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode the size of the secret file extension from the stego image */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode an integer value (e.g., size) from LSBs of the image buffer */
Status decode_int_from_lsb(int *size, char *image_buffer);

/* Decode the actual secret file extension from the stego image */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode the size of the secret file from the stego image */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode the secret file data (content) from the stego image */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode a single byte from the LSBs of 8 bytes in the image buffer */
Status decode_byte_from_lsb(char *ch, char *buffer);

/* Close all opened files related to decoding */
void close_decode_files(DecodeInfo *decInfo);

#endif
