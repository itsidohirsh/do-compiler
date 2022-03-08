#include <stdlib.h>
#include <stdio.h>

#include "../io/io.h"
#include "../error_handler/error_handler.h"

char* read_file(const char* filename)
{
    char* buffer = 0;
    unsigned long long length;

    // Opening the file
    FILE* fp = fopen(filename, "rb");

    // Check for NULL file pointer
    if (fp == NULL)
    {
        red();
        printf("[I/O Error] ");
        reset();
        printf("Could not read file ");
        cyan();
        printf("%s\n", filename);
        reset();
        exit(1);
    }

    // Using fseek & ftell to get length of file
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);

    // Go to start of file again
    fseek(fp, 0, SEEK_SET);

    // Allocate the buffer size according to the length of the file + 1 for \0
    buffer = malloc((length + 1) * sizeof(char));

    // Check for allocation error
    if (buffer == NULL)
    {
        // Closing the file
        fclose(fp);

        error_handler_report_alloc();
    }

    // Copying the contents of the file to the buffer
    fread(buffer, 1, length, fp);
    // Ending the source code buffer with '\0'
    buffer[length] = '\0';

    // Closing the file
    fclose(fp);

    return buffer;
}
