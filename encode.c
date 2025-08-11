/*
Documentation
Name       :G Gangadhar
Date       :30/07/2025
Description:Steganography project
*/
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "encode.h"
#include "types.h"

// Main encoding function that performs all encoding steps
Status do_encoding(EncodeInfo *encInfo)
{
    printf("[INFO] Opening requried files\n");
    if ((open_files(encInfo)) == e_failure)
    {
        printf("ERROR: ❌ Error opening files\n");
        return e_failure;
    }
    // File open confirmations
    // printf("[INFO] Opened %s \n", encInfo->src_image_fname);
    // printf("[INFO] Opened %s \n", encInfo->secret_fname);
    // printf("[INFO] Opened %s \n", encInfo->stego_image_fname);
    printf("[INFO] ✅ Done\n\n");
    printf("──────────────────────────────────────────────\n");
    printf("[INFO] 🔐 Encoding Procedure Started \n");
    printf("──────────────────────────────────────────────\n");
    // Capacity check
    printf("[INFO] Checking for %s capacity to handle %s\n", encInfo->src_image_fname, encInfo->secret_fname);

    if (check_capacity(encInfo) == e_failure)
    {
        printf("ERROR: ❌ Insufficient image capacity for encoding\n");
        return e_failure;
    }
    printf("[INFO] Sufficient space available\n");
    printf("[INFO] ✅ Done. Found OK\n\n");

    // Copy BMP header
    printf("[INFO] Copying Image Header\n");
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: ❌ Failed to copy BMP header\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Encode magic string
    printf("[INFO] Encoding Magic String Signature\n");
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("ERROR: ❌ Failed to encode magic string\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Encode secret file extension size
    printf("[INFO] Encoding %s File Extension Size\n", encInfo->secret_fname);
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("ERROR: ❌ Failed to encode file extension size\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Encode secret file extension
    printf("[INFO] Encoding %s File Extension \n", encInfo->secret_fname);
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("ERROR: ❌ Failed to encode file extension\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Encode secret file size
    printf("[INFO] Encoding %s File Size\n", encInfo->secret_fname);
    if (encode_secret_file_size(get_file_size(encInfo->fptr_secret), encInfo) == e_failure)
    {
        printf("ERROR: ❌ Failed to encode secret file size\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Encode actual file data
    printf("[INFO] Encoding %s File Data\n", encInfo->secret_fname);
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        fprintf(stderr, "ERROR: ❌ Failed to encode secret file data\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Copy remaining image data
    printf("[INFO] Copying Left Over Data\n");
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: ❌ Failed to copy remaining image data\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    return e_success;
}

// Identify the operation type (encode or decode)
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

// Validate and read encoding arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = NULL;
    encInfo->fptr_stego_image = NULL;
    encInfo->fptr_secret = NULL;

    // Check source image file extension
    char *bmp = strstr(argv[2], ".bmp");
    if ((bmp != NULL) && (strcmp(bmp, ".bmp") == 0))
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("ERROR: ❌ Invalid source image name\n");
        printf("[INFO] Source image must have a .bmp extension\n");
        return e_failure;
    }

    // Check secret file extension
    char *sh = strstr(argv[3], ".sh");
    char *txt = strstr(argv[3], ".txt");
    char *c = strstr(argv[3], ".c");
    if ((c != NULL) && (strcmp(c, ".c") == 0))
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".c");
    }
    else if ((txt != NULL) && (strcmp(txt, ".txt") == 0))
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".txt");
    }
    else if ((sh != NULL) && (strcmp(sh, ".sh") == 0))
    {
        encInfo->secret_fname = argv[3];
        strcpy(encInfo->extn_secret_file, ".sh");
    }
    else
    {
        printf("ERROR: ❌ Unsupported secret file extension\n");
        printf("[INFO] Secret file must have .c, .txt, or .sh extension\n");
        return e_failure;
    }

    // Optional stego image name
    if (argv[4] == NULL)
    {
        printf("[INFO] Output image name not provided. Creating default file name as 'stego.bmp'\n");
        encInfo->stego_image_fname = "stego.bmp";
    }
    else
    {
        char *bmp2 = strstr(argv[4], ".bmp");
        if ((bmp2 != NULL) && (strcmp(bmp, ".bmp") == 0))
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            printf("ERROR: ❌ %s must contain the .bmp extension\n", argv[4]);
            return e_failure;
        }
    }
    return e_success;
}

// Check if the image has enough capacity
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    int encode_size = 54 + ((strlen(MAGIC_STRING) + 4 + strlen(encInfo->extn_secret_file) +
                             4 + get_file_size(encInfo->fptr_secret)) *
                            8);
    if (encInfo->image_capacity > encode_size)
    {
        return e_success;
    }
    else
        return e_failure;
}

// Get the size of a file
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

// Copy the 54-byte BMP header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char cptr[54];
    rewind(fptr_src_image);
    if (fread(cptr, 54, 1, fptr_src_image) != 1)
    {
        fprintf(stderr, "ERROR: ❌ Unable to read BMP header from the source file.\n");
        return e_failure;
    }
    if (fwrite(cptr, 54, 1, fptr_dest_image) != 1)
    {
        fprintf(stderr, "ERROR: ❌ Unable to write BMP header to the destination file.\n");
        return e_failure;
    }
    return e_success;
}

// Encode the predefined magic string
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    if (encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
        return e_success;
    return e_failure;
}

// Generic function to encode a string of data into image
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
        if (fread(buffer, 8, 1, fptr_src_image) != 1)
        {
            printf("ERROR: ❌ Failed to read source image while encoding data\n");
            return e_failure;
        }
        encode_byte_to_lsb(data[i], buffer);
        if (fwrite(buffer, 8, 1, fptr_stego_image) != 1)
        {
            fprintf(stderr, "ERROR: ❌ Failed to write encoded data to destination image\n");
            return e_failure;
        }
    }
    return e_success;
}

// Encode a single byte into 8 LSBs of 8 bytes
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i, get, clear, j = 7;
    for (i = 0; i <= 7; i++)
    {
        get = (data >> j) & 1;
        clear = image_buffer[i] & ~(1);
        image_buffer[i] = get | clear;
        j--;
    }
    return e_success;
}

// Encode the length of the secret file extension
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    char buffer[32];
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
    {
        fprintf(stderr, "ERROR: ❌ Failed to read 32 bytes from the source image for encoding extension size.\n");
        return e_failure;
    }
    encode_int_to_lsb(extn_size, buffer);
    if (fwrite(buffer, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        fprintf(stderr, "ERROR: ❌ Failed to write 32 bytes to the stego image after encoding extension size.\n");
        return e_failure;
    }
    return e_success;
}

// Encode an integer value into 32 LSBs of 32 bytes
Status encode_int_to_lsb(int data, char *image_buffer)
{
    int i, j = 31, get, clear;
    for (i = 0; i <= 31; i++)
    {
        get = (data >> j) & 1;
        clear = image_buffer[i] & (~1);
        image_buffer[i] = get | clear;
        j--;
    }
    return e_success;
}

// Encode the actual secret file extension
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    if (encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
        return e_success;
    return e_failure;
}

// Encode the size of the secret file
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    encInfo->size_secret_file = file_size;
    char buffer[32];
    if (fread(buffer, 32, 1, encInfo->fptr_src_image) != 1)
    {
        fprintf(stderr, "ERROR: ❌ Unable to read 32 bytes from the source image for encoding secret file size.\n");
        return e_failure;
    }
    encode_int_to_lsb(file_size, buffer);
    if (fwrite(buffer, 32, 1, encInfo->fptr_stego_image) != 1)
    {
        fprintf(stderr, "ERROR: ❌ Failed to write encoded data to the stego image file (expected 32 bytes).\n");
        return e_failure;
    }
    return e_success;
}

// Encode the content of the secret file
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_secret);
    char buffer[encInfo->size_secret_file];
    if (fread(buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret) != 1)
    {
        fprintf(stderr, "ERROR: ❌ Failed to read %d bytes from secret file.\n", encInfo->size_secret_file);
        return e_failure;
    }
    if (encode_data_to_image(buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "ERROR: ❌ Failed to encode secret file data into the stego image.\n");
        return e_failure;
    }
    return e_success;
}

// Copy remaining bytes from source to stego image
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while ((fread(&ch, 1, 1, fptr_src)) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}
