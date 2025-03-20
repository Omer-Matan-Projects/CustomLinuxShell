#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

#include <math.h>       // Required for pow

int main(int argc, char* argv[])
{
    // The return value of this program indicates the result of power calculation.
    // If the result is less than or equal to 0, it returns 0.
    // If the result is 255 or greater, it returns 255.
    // Otherwise, it returns the calculated result.

    int base = atoi(argv[1]);
    int exponent = atoi(argv[2]);
    int res = (int)pow(base, exponent);

    printf("%d\n", res);

    if (res <= 0)
    {
        return 0; // Return 0 if the result is zero or negative
    }
    else if (res >= 255)
    {
        return 255; // Return 255 if the result is 255 or greater
    }
    else
    {
        return res; // Return the calculated result if it is within the range of 1 to 254
    }
}