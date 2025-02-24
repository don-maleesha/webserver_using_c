#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "utils.h" // custom header file

#define PORT 8080 // port number

int main(int argc, char const *argv[])
{
    int server_fd; // server file descriptor
    struct sockaddr_in server_addr; // server address structure

    // create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed"); // print an error message
        exit(EXIT_FAILURE); // exit the program
    }

    server_addr.sin_family = AF_INET; // set the address family
    server_addr.sin_addr.s_addr = INADDR_ANY; // set the address to any interface
    server_addr.sin_port = htons(PORT); // set the port number

    // bind the socket to the address and port number
    if (bind(server_fd,
             (struct sockaddr *)&server_addr, // cast the server address structure to a sockaddr structure
             sizeof(server_addr)) < 0) // get the size of the server address structure
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT); // print a message

    // accept incoming connections
    while (1)
    {
        struct sockaddr_in client_addr; // client address structure
        socklen_t client_addr_len = sizeof(client_addr); // get the size of the client address structure
        int *client_fd = malloc(sizeof(int)); // allocate memory for the client file descriptor

        // accept the connection
        if ((*client_fd = accept(server_fd,
                                 (struct sockaddr *)&client_addr, // cast the client address structure to a sockaddr structure
                                 &client_addr_len)) < 0) // get the size of the client address structure
        {
            perror("accept failed");
            continue;
        }

        pthread_t thread_id; // thread ID
        pthread_create(&thread_id, NULL, handle_client, (void *)client_fd); // create a new thread to handle the client
        pthread_detach(thread_id); // detach the thread
    }

    close(server_fd);
    return 0;
}