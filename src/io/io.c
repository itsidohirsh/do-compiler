#include <stdlib.h>
#include <stdio.h>

#include "io.h"
#include "../general/general.h"


char* io_read_file(char* file_name)
{
    char* buffer = 0;
    unsigned long long length;

    // Opening the file
    FILE* fp = fopen(file_name, "rb");
    if (fp == NULL) exit_file_io_error(file_name);

    // Using fseek & ftell to get length of file
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);

    // Go to start of file again
    fseek(fp, 0, SEEK_SET);

    // Allocate the buffer size according to the length of the file + 1 for \0
    buffer = (char*) calloc(length + 1, sizeof(char));
    if (buffer == NULL)
    {
        fclose(fp);
        exit_memory_error(__FILE__, __LINE__);
    }

    // Copying the contents of the file to the buffer
    fread(buffer, 1, length, fp);
    // Ending the source code buffer with '\0'
    buffer[length] = '\0';

    // Closing the file
    fclose(fp);

    return buffer;
}
