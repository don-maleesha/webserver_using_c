# 🌐 Web Server using C

This project is a simple web server implemented in C. It serves HTML pages and other web-related assets over HTTP. The server handles GET requests and responds with the appropriate resources.

# 📂 Directory Structure

    /webserver_using_c
    │-- /lib     # Custom library files
    │-- /src     # HTML pages and web assets (images, videos, etc.)
    │-- Makefile # Makefile
    │-- server.c # Main server file handling user requests
    │-- utils.h  # Utility functions for server operations

# ✨ Features

    Serves static HTML, CSS, JS, and media files.
    
    Handles HTTP requests and responses.
    
    Uses multithreading to support multiple users.
    
    MIME type detection for correct content delivery.
    
    URL decoding for special characters.

# 🔍 Implementation Details

File Helpers

    get_file_descriptor(): Opens and returns a file descriptor for requested files.
    
    get_file_extension(): Extracts the file extension from the requested resource.

HTTP Helpers

    get_mime_type(): Maps file extensions to their respective MIME types.
    
    url_decode(): Decodes URL-encoded characters (e.g., %20 -> space).
    
    build_http_response(): Constructs HTTP responses with headers and content.

Server Utilities

    handle_client(): Processes client requests, reads files, and returns responses.
    
    Uses sys/socket.h and netinet/in.h for socket programming (Linux/macOS only).
    
    Multithreading using pthread for concurrent request handling.

# 🚀 Running the Server

Compilation (Linux/macOS)

Use the Makefile for easier builds:

    $ make

# Start the Server

Run the server executable:

    $ ./server

Accessing the Server

Open a web browser and navigate to:

    http://localhost:8080

To access specific pages:

    http://localhost:8080/about.html

# Notes

    Ensure your HTML files are stored in the /src directory.
    
    MIME types should be properly mapped in http_helper.h.
    
    Close file streams and free memory to prevent leaks.
    
    This server is designed for Linux/macOS and may not work on Windows without modifications (e.g., using Winsock).

