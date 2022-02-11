#include <stdlib.h>
#include <stdio.h>

#include "../include/io.h"

char* read_file(const char* filename)
{
    char* buffer = 0;
    unsigned long long length;

    // Opening the file
    FILE* fp = fopen(filename, "rb");

    // Check for NULL file pointer
    if (fp == NULL)
    {
        printf("Could not read file `%s`\n", filename);
        exit(1);
    }

    // Using fseek & ftell to get length of file
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);

    // Go to start of file again
    fseek(fp, 0, SEEK_SET);

    // Allocate the buffer size according to the length of the file
    buffer = calloc(length, sizeof(char));

    // Check for NULL pointer after allocation
    if (buffer != NULL)
    {
        // Copying the contents of the file to the buffer
        fread(buffer, 1, length, fp);
    }

    // Closing the file
    fclose(fp);

    return buffer;
}
