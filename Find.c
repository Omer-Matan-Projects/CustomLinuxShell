#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

// This program takes a filename and a string from the command-line arguments.
// It searches for the string in the file. If the string is found, it prints "WIN".
// Otherwise, it prints "Try Again".

int main(int argc, char* argv[])
{
    int fd_file, rbytes;
    char buffer[1024];
    char* filename = argv[1];
    char* find_str = argv[2];
    int find_len = strlen(find_str); // Length of the search string
    int match_pos = 0; // Position of the current match in the search string

    // Open the file for reading
    if ((fd_file = open(filename, O_RDONLY)) == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // Read the file in chunks and search for the string
    while ((rbytes = read(fd_file, buffer, sizeof(buffer))) > 0)
    {
        // Iterate over each byte read from the file
        for (int i = 0; i < rbytes; ++i)
        {
            // Check if the current byte matches the current character in find_str
            if (buffer[i] == find_str[match_pos]) 
            {
                match_pos++; // Move to the next character in find_str

                // If a complete match is found
                if (match_pos == find_len) 
                {
                    printf("WIN\n");

                    // Close the file descriptor
                    if (close(fd_file) == -1)
                    {
                        perror("close failed");
                        exit(EXIT_FAILURE);
                    }

                    return 0; // Exit the program with success status
                }
            } 
            else 
            {
                match_pos = 0; // Reset match position if characters do not match
            }
        }
    }

    // Check for read errors
    if (rbytes == -1)
    {
        perror("read failed"); // Print error message if reading fails
        
        // Close the file descriptor
        if (close(fd_file) == -1)
        {
            perror("close failed");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE); // Exit the program with failure status
    }

    printf("Try Again\n"); // Print "Try Again" to the standard output if no match was found

    // Close the file descriptor
    if (close(fd_file) == -1)
    {
        perror("close failed");
        exit(EXIT_FAILURE);
    }

    return 0;  // Exit the program with success status
}