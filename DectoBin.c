#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

// This program converts an integer to its binary representation and prints it.
// It returns 0 if successful.

int main(int argc, char* argv[])
{
    int binary[32]; // Array to store binary digits (up to 32 bits for int)
    int j, i = 0;

    int n = atoi(argv[1]);

    if (n < 0) // Handle negative numbers using two's complement method
    {
        int mask = 1 << (sizeof(int) * 8 - 1); // Mask for the sign bit
        n = (~n) + 1; // Compute the two's complement
        n = n | mask; // Set the sign bit to 1
    }

    for (; n > 0; n /= 2) // Convert decimal to binary using a regular loop
    {
        binary[i++] = n % 2; // Store the remainder in the binary array
    }

    for (j = i - 1; j >= 0; j--) // Print the binary digits in reverse order
    {
        printf("%d", binary[j]);
    }
    printf("\n");

    return 0; // Return 0 to indicate successful execution
}