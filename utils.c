#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#include "./lib/file_helper.h" // customer header for file operations
#include "./lib/http_helper.h" // customer header for http operations

#define BUFFER_SIZE 104857600 // 100 MB
#define SRC_DIR "./src/" // source directory

// function to build an HTTP response
void build_http_response(const char *file_name, const char *file_ext, char *response, size_t *response_len)
{
    printf("Considered FILE NAME: %s \n", file_name);

    int file_fd = open_file(SRC_DIR, file_name); // open the file

    // check if the file is not found
    if (file_fd == -1)
    {
        file_fd = open_file(SRC_DIR, "404.html"); // open the 404 file

        // check if the 404 file is not found
        if (file_fd == -1)
        {
            // build a 404 response
            snprintf(response, BUFFER_SIZE,
                     "HTTP/1.1 404 Not Found\r\n"
                     "Content-Type: text/plain\r\n"
                     "\r\n"
                     "404 Not Found");
            *response_len = strlen(response); // set the response length
            return;
        }
        else
        {
            file_name = "404.html"; // set the file name to 404.html
            file_ext = "html"; // set the file extension to html
        }
    }

    struct stat file_stat; // file stat structure
    fstat(file_fd, &file_stat); // get the file stat
    off_t file_size = file_stat.st_size; // get the file size
    printf("FILE_DESC: %d  SIZE: %li \n", file_fd, file_size);

    const char *mime_type = get_mime_type(file_ext); // get the mime type
    char *header = (char *)malloc(BUFFER_SIZE * sizeof(char)); // allocate memory for the header

    // build the HTTP header
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %li\r\n"
             "\r\n",
             (strcmp(file_name, "404.html") == 0) ? "404 Not Found" : "200 OK",
             mime_type,
             file_size);

    *response_len = 0; // initialize the response length
    memcpy(response, header, strlen(header)); // copy the header to the response
    *response_len += strlen(header); // increment the response length

    ssize_t bytes_read; // bytes read
    // read the file content
    while ((bytes_read = read(file_fd, response + *response_len, BUFFER_SIZE - *response_len)) > 0)
    {
        *response_len += bytes_read;
    }

    free(header);
    close(file_fd);
}

// function to handle a client request
void *handle_client(void *arg)
{
    int client_fd = *((int *)arg); // get the client file descriptor
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char)); // allocate memory for the buffer

    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0); // receive data from the client

    // check if data is received
    if (bytes_received > 0)
    {
        regex_t regex; // regex structure
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED); // compile the regex pattern
        regmatch_t matches[2]; // array to store matches

        // check if the regex pattern matches the request
        if (regexec(&regex, buffer, 2, matches, 0) == 0)
        {
            buffer[matches[1].rm_eo] = '\0'; // add null terminator
            const char *url_encoded_file_name = buffer + matches[1].rm_so; // get the URL encoded file name
            char *file_name = url_decode(url_encoded_file_name); // decode the URL encoded file name

            // check if the file name is empty
            if (strlen(file_name) == 0)
            {
                strcpy(file_name, "index.html"); // set the file name to index.html
            }

            printf("Requested file: %s\n", file_name);

            const char *file_ext = extract_extension(file_name); // extract the file extension

            char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char)); // allocate memory for the response
            size_t response_len; // response length
            build_http_response(file_name, file_ext, response, &response_len); // build the HTTP response

            send(client_fd, response, response_len, 0); // send the response

            free(response); // free the response
            free(file_name);    // free the file name
        }

        regfree(&regex); // free the regex structure
    }

    close(client_fd); // close the client file descriptor
    free(arg); // free the argument
    free(buffer); //
    return NULL;
}