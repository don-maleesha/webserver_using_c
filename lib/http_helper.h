#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

const char *get_mime_type(const char *file_name);

// function to determine the mime type of a file based on its extension
const char *get_mime_type(const char *file_ext)
{
    // check the file extension and return the appropriate mime type
    if (strcasecmp(file_ext, "html") == 0 || strcasecmp(file_ext, "htm") == 0)
    {
        return "text/html";
    }
    else if (strcasecmp(file_ext, "txt") == 0)
    {
        return "text/plain";
    }
    else if (strcasecmp(file_ext, "jpg") == 0 || strcasecmp(file_ext, "jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcasecmp(file_ext, "png") == 0)
    {
        return "image/png";
    }
    else
    {
        return "application/octet-stream";
    }
}

// function to decode a URL encoded string
char *url_decode(const char *src)
{
    size_t src_len = strlen(src); // get the length of the source string
    char *decoded = (char*)malloc(src_len + 1); // allocate memory for the decoded string
    size_t decoded_len = 0; // initialize the decoded length

    // decode %2x to hex
    for (size_t i = 0; i < src_len; i++)
    {
        // check if the character is a % sign and there are at least two characters after it
        if (src[i] == '%' && i + 2 < src_len)
        {
            unsigned int hex_val; // variable to store the hex value
            sscanf(src + i + 1, "%2x", &hex_val); // read the next two characters as a hex value
            decoded[decoded_len++] = hex_val; // add the hex value to the decoded string
            i += 2; // skip the next two characters
        }
        else
        {
            // add the character as is
            decoded[decoded_len++] = src[i];
        }
    }

    // add null terminator
    decoded[decoded_len] = '\0';
    return decoded;
}