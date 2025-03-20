#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

int main(int argc, char* argv[])
{
    int fd_file, rbytes;
    char fileName[256], buffer[1024];

    // Construct the file path
    snprintf(fileName, sizeof(fileName), "Commands/%s/%s_Commands.txt", argv[1], argv[1]);

    // Open the file for reading
    if ((fd_file = open(fileName, O_RDONLY)) == -1)
    {
        perror("open failed"); // Print error message if open fails
        exit(EXIT_FAILURE);    // Exit the program with failure status
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
        perror("read failed"); // Print error message if read fails
        exit(EXIT_FAILURE);    // Exit the program with failure status
    }

    // Close the file descriptor
    if (close(fd_file) == -1)
    {
        perror("close failed"); // Print error message if close fails
        exit(EXIT_FAILURE);     // Exit the program with failure status
    }

    return 0; // Return 0 to indicate successful execution
}