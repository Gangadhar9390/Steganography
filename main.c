/*
Documentation
Name       :G Gangadhar
Date       :30/07/2025
Description:Steganography project
*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

int main(int argc, char *argv[])
{
    // Declare structures for encoding and decoding
    EncodeInfo encodeInfo;
    DecodeInfo decodeInfo;

    // Check if minimum number of arguments is provided
    if (argc < 2)
    {
        // Print usage info for both encoding and decoding
        printf("Usage:\n");
        printf("Encoding: ./a.out -e <image_file.bmp> <secret_file.txt|.c|.sh> [optional_image.bmp]\n");
        printf("Decoding: ./a.out -d <image_file.bmp> [optional_secret_file]\n");
        return 1;
    }

    // Get the type of operation (encode or decode)
    OperationType op_type = check_operation_type(argv);

    // If encoding operation
    if (op_type == e_encode)
    {
        // Check if correct number of arguments for encoding
        if (argc >= 4 && argc <= 5)
        {
            // Validate encoding arguments
            printf("[INFO] Validating encoding arguments\n");
            if (read_and_validate_encode_args(argv, &encodeInfo) == e_failure)
            {
                fprintf(stderr, "Error: ❌ Invalid encoding arguments.\n");
                return e_failure;
            }
            printf("[INFO] ✅ Done\n\n");
            // Start encoding process
            if (do_encoding(&encodeInfo) == e_failure)
            {
                // Close files and handle failure
                close_encode_files(&encodeInfo);
                fprintf(stderr, "Error: ❌ Encoding failed.\n");
                return e_failure;
            }

            // Close files and confirm success
            close_encode_files(&encodeInfo);
            printf("──────────────────────────────────────────────\n");
            printf("[INFO]  ✅ Encoding Completed Successfully!\n");
            printf("──────────────────────────────────────────────\n");
            return 0;
        }
        else
        {
            // Handle incorrect argument count for encoding
            fprintf(stderr, "Error: ❌ Invalid number of arguments for encoding.\n");
            printf("Usage:\n");
            printf("Encoding: ./a.out -e <image_file.bmp> <secret_file.txt|.c|.sh> [optional_image.bmp]\n");
            return e_failure;
        }
    }

    // If decoding operation
    else if (op_type == e_decode)
    {
        // Check if correct number of arguments for decoding
        if (argc >= 3 && argc <= 4)
        {
            // Validate decoding arguments
            printf("[INFO] Validating decoding arguments\n");
            if (read_and_validate_decode_args(argv, &decodeInfo) == e_failure)
            {
                fprintf(stderr, "Error: ❌ Invalid decoding arguments.\n");
                return e_failure;
            }
            printf("[INFO] ✅ Done\n\n");
            // Start decoding process
            if (do_decoding(&decodeInfo) == e_failure)
            {
                // Close files and handle failure
                close_decode_files(&decodeInfo);
                fprintf(stderr, "Error: ❌ Decoding failed.\n");
                return e_failure;
            }
            // Close files and confirm success
            close_decode_files(&decodeInfo);
            printf("──────────────────────────────────────────────\n");
            printf("[INFO] ✅ Decoding Completed Successfully!\n");
            printf("──────────────────────────────────────────────\n");
            return 0;
        }
        else
        {
            // Handle incorrect argument count for decoding
            fprintf(stderr, "Error:  ❌ Invalid number of arguments for decoding.\n");
            printf("Usage:\n");
            printf("Decoding: ./a.out -d <image_file.bmp> [optional_secret_file]\n");
            return e_failure;
        }
    }

    // If invalid operation type (not -e or -d)
    else
    {
        fprintf(stderr, "Error: ❌ Invalid operation type. Use -e or -d.\n");
        printf("Usage:\n");
        printf("Encoding: ./a.out -e <image_file.bmp> <secret_file.txt|.c|.sh> [optional_image.bmp]\n");
        printf("Decoding: ./a.out -d <image_file.bmp> [optional_secret_file]\n");
        return e_failure;
    }
}
