#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

int open_file(const char *directory, const char *file_name);

// function to open a file
int open_file(const char *directory, const char *file_name)
{
    char file_path[100]; // file path
    strcpy(file_path, directory); // copy directory to file_path
    strcat(file_path, file_name); // concatenate file_name to file_path

    int file_descriptor = open(file_path, O_RDONLY); // open file in read-only mode

    return file_descriptor;
}

// function to extract extension from file name
const char *extract_extension(const char *file_name)
{
    const char *extension = strrchr(file_name, '.'); // get the last occurrence of '.' in file_name

    // if extension is not found or extension is same as file_name
    if(!extension || extension == file_name)
    {
        return "";
    }

    return extension + 1;
}