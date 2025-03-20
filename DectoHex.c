#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

// This program converts a decimal number to its hexadecimal representation and prints it.
// It returns 0 if the hexadecimal value is less than or equal to 0.
// It returns 255 if the hexadecimal value is greater than or equal to 255.
// Otherwise, it returns the hexadecimal value itself.

int main(int argc, char* argv[])
{
    int decimal_number = atoi(argv[1]);  // Convert command line argument to integer
    int hex_value = decimal_number;  // Store decimal_number in hex_value

    printf("%X\n", hex_value);  // Print hexadecimal representation of hex_value

    // Return appropriate value based on the range of hex_value
    if (hex_value <= 0)
    {
        return 0;  // Return 0 if hex_value is less than or equal to 0
    }
    else if (hex_value >= 255)
    {
        return 255;  // Return 255 if hex_value is greater than or equal to 255
    }
    else
    {
        return hex_value;  // Return the hexadecimal value itself if it's in the range (0, 255)
    }
}