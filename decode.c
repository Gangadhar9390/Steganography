/*
Documentation
Name       :G Gangadhar
Date       :30/07/2025
Description:Steganography project
*/
#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

// Main decoding function
Status do_decoding(DecodeInfo *decInfo)
{
    printf("──────────────────────────────────────────────\n");
    printf("[INFO] 🔓 Decoding Procedure Started\n");
    printf("──────────────────────────────────────────────\n");
    printf("[INFO] Opening required files\n");

    // Open stego image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)
    {
        fprintf(stderr, "ERROR: ❌ Failed to open stego image file: %s\n", decInfo->stego_image_fname);
        perror("ERROR");
        return e_failure;
    }
    printf("[INFO] Opened %s\n", decInfo->stego_image_fname);
    printf("[INFO] ✅ Done\n\n");

    // Decode and validate the magic string
    printf("[INFO] Decoding Magic String Signature\n");
    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("Magic string is not present\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Decode extension size of secret file
    printf("[INFO] Decoding Output File Extension size\n");
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("Error: ❌ Failed to get extension size\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Decode actual extension of secret file (e.g. .txt, .c, etc.)
    printf("[INFO] Decoding Output File Extension\n");
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("Error: ❌ Falied to get extension\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Create output file for secret data

    printf("[INFO] Creating output file: %s\n", decInfo->secret_fname);

    // opening the secret file
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_fname);
        return e_failure;
    }
    printf("[INFO] ✅ Done. Opened all required files\n\n");

    // Decode size of the secret file
    printf("[INFO] Decoding size of the output file: %s\n", decInfo->secret_fname);
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        fprintf(stderr, "Error: ❌ Falied to get file size\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");

    // Decode and extract secret file data
    printf("[INFO] Decoding secret file content\n");
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        fprintf(stderr, "Error: ❌ Failed at decoding file data\n");
        return e_failure;
    }
    printf("[INFO] ✅ Done\n\n");
    return e_success;
}

// Validate decoding arguments and extract filenames
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = NULL;
    decInfo->fptr_secret = NULL;

    // Validate input BMP file
    char *bmp = strstr(argv[2], ".bmp");
    if ((bmp != NULL) && strcmp(bmp, ".bmp") == 0)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("Error: ❌ Enter file name with <file_name.bmp>\n");
        return e_failure;
    }

    // If no output filename provided, use default
    if (argv[3] == NULL)
    {
        printf("[INFO] Output filename not mentioned. Default name as 'secret_file'\n ");
        strcpy(decInfo->secret_fname, "secret_file");
    }
    else
    {
        if (strchr(argv[3], '.') != NULL)
        {
            printf("Error: ❌ Do not include file extension in output filename\n");
            printf("[INFO] Provide filename without extension <output_file>\n");
            return e_failure;
        }
        strcpy(decInfo->secret_fname, argv[3]);
    }
    return e_success;
}

// Decode the magic string to verify data presence
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET); // Skip BMP header
    int i;
    char buffer[8], ch, magicString[strlen(MAGIC_STRING) + 1];
    for (i = 0; i < strlen(MAGIC_STRING); i++)
    {
        if (fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
            fprintf(stderr, "ERROR : ❌ Failed to read from %s while decoding magic string.\n", decInfo->stego_image_fname);
            return e_failure;
        }
        decode_byte_from_lsb(&ch, buffer);
        magicString[i] = ch;
    }
    magicString[i] = '\0';

    // Compare decoded string with MAGIC_STRING constant
    if (strcmp(MAGIC_STRING, magicString) == 0)
    {
        return e_success;
    }
    return e_failure;
}

// Decode one byte using LSBs of 8 bytes
Status decode_byte_from_lsb(char *ch, char *buffer)
{
    int i, get, j = 7;
    *ch = 0;
    for (i = 0; i <= 7; i++)
    {
        get = buffer[i] & 1;
        *ch = *ch | (get << j);
        j--;
    }
    return e_success;
}

// Decode 4-byte integer (like file size or extn size) from 32 LSBs
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    if (fread(buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        printf("ERROR: ❌ Failed to read %s while decoding extension size\n", decInfo->stego_image_fname);
        return e_failure;
    }
    decode_int_from_lsb(&decInfo->secret_file_extn_size, buffer);
    return e_success;
}

// Convert 32 LSBs to integer
Status decode_int_from_lsb(int *size, char *image_buffer)
{
    *size = 0;
    int i, get, j = 31;
    for (i = 0; i <= 31; i++)
    {
        get = image_buffer[i] & 1;
        *size |= (get << j);
        j--;
    }
    return e_success;
}

// Decode and attach extension to the output file name
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extension[decInfo->secret_file_extn_size + 1];
    char buffer[8], ch;
    int i;
    for (i = 0; i < decInfo->secret_file_extn_size; i++)
    {
        if (fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
            printf("ERROR: ❌ Failed to read %s while decoding file extension \n", decInfo->stego_image_fname);
            return e_failure;
        }
        decode_byte_from_lsb(&ch, buffer);
        extension[i] = ch;
    }
    extension[i] = '\0';

    // Append decoded extension to filename
    strcat(decInfo->secret_fname, extension);
    return e_success;
}

// Decode the full size of secret file
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];
    if (fread(buffer, 32, 1, decInfo->fptr_stego_image) != 1)
    {
        printf("ERROR: ❌ Failed to read %s while decoding secret file size\n", decInfo->stego_image_fname);
        return e_failure;
    }
    decode_int_from_lsb(&decInfo->size_secret_file, buffer);
    return e_success;
}

// Decode and write the entire secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8], ch;
    int i;
    for (i = 0; i < decInfo->size_secret_file; i++)
    {
        if (fread(buffer, 8, 1, decInfo->fptr_stego_image) != 1)
        {
            printf("ERROR: ❌ Failed to read %s while decoding file data\n", decInfo->stego_image_fname);
            return e_failure;
        }
        decode_byte_from_lsb(&ch, buffer);

        // Write decoded byte to secret file
        if (fwrite(&ch, 1, 1, decInfo->fptr_secret) != 1)
        {
            printf("ERROR: ❌ Failed to write %s into %s decoding data\n", decInfo->stego_image_fname, decInfo->secret_fname);
            return e_failure;
        }
    }
    return e_success;
}

// Close opened files after decoding
void close_decode_files(DecodeInfo *decInfo)
{
    int flag = 0;
    if (decInfo->fptr_stego_image != NULL)
    {
        flag = 1;
        fclose(decInfo->fptr_stego_image);
        printf("[INFO] Closing %s file\n", decInfo->stego_image_fname);
    }

    if (decInfo->fptr_secret != NULL)
    {
        flag = 1;
        fclose(decInfo->fptr_secret);
        printf("[INFO] Closing %s file\n", decInfo->secret_fname);
    }
    if (flag)
    {
        printf("[INFO] ✅ Done. Successfully closed files\n");
    }
}
