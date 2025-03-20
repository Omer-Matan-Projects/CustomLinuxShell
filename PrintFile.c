#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

// This program reads the contents of a file specified by a command-line argument and prints them to the standard output.

int main(int argc, char* argv[])
{
    int fd_file, rbytes;
    char buffer[1024];
    char* filename = argv[1];

    // Open the file for reading
    if ((fd_file = open(filename, O_RDONLY)) == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    // Read and print contents of the file in chunks
    while ((rbytes = read(fd_file, buffer, sizeof(buffer))) > 0)
    {
        // Print only the valid bytes read (to avoid mismatch between the size of the buffer and the actual size of data read from the file)
        for (int i = 0; i < rbytes; i++)
        {
            printf("%c", buffer[i]);
        }
    }

    // Check for read errors
    if (rbytes == -1)
    {
        perror("read failed");

        // Close the file descriptor
        if (close(fd_file) == -1)
        {
            perror("close failed");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_FAILURE);
    }

    printf("\n");

    // Close the file descriptor
    if (close(fd_file) == -1)
    {
        perror("close failed");
        exit(EXIT_FAILURE);
    }

    return 0; // Exit the program with success status
}