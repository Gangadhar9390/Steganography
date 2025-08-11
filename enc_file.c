/*
Documentation
Name       :G Gangadhar
Date       :30/07/2025
Description:Steganography project
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    if ((fread(&width, sizeof(int), 1, fptr_image) != 1) ||
        (fread(&height, sizeof(int), 1, fptr_image) != 1)) // Read the height (an int)
    {
        fprintf(stderr, "Error reading width and height\n");
        return 0;
    }
    // printf("width = %u\n", width);

    // printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: ❌Unable to open file %s\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: ❌ Unable to open file %s\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: ❌ Unable to open file %s\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}
void close_encode_files(EncodeInfo *encInfo)
{
    int flag = 0; // Flag to check if any file was closed

    // Close source image file if open
    if (encInfo->fptr_src_image != NULL)
    {
        flag = 1;
        fclose(encInfo->fptr_src_image);
        printf("[INFO] Closing %s file\n", encInfo->src_image_fname);
    }

    // Close stego image file if open
    if (encInfo->fptr_stego_image != NULL)
    {
        flag = 1;
        fclose(encInfo->fptr_stego_image);
        printf("[INFO] Closing %s file\n", encInfo->stego_image_fname);
    }

    // Close secret file if open
    if (encInfo->fptr_secret != NULL)
    {
        flag = 1;
        fclose(encInfo->fptr_secret);
        printf("[INFO] Closing %s file\n", encInfo->secret_fname);
    }

    // Print success message if any file was closed
    if (flag)
    {
        printf("[INFO] ✅ Done. Successfully closed files\n");
    }
}
