#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

// This program takes an integer as a command-line argument and returns the number of 1 bits.

int main(int argc, char* argv[])
{
    int count = 0, n;
    n = atoi(argv[1]);  

    if (n < 0) // Check if n is negative
    {
        n = ~n + 1; // Convert n to its two's complement representation (~ = NOT)
    }
    while (n) // Loop to count the number of 1 bits in the binary representation of n 
    {
        count += n & 1; // Increment count if the least significant bit of n is 1
        n = n >> 1; // Right shift n by 1 bit to check the next bit
    }

    printf("%d\n", count);

    return count;
}