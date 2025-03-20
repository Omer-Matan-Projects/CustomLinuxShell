#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

#include <math.h>       // Required for sqrt

int main(int argc, char* argv[])
{
    // The return value of this program indicates the result of the square root calculation.
    // If the number is negative, it returns 0.
    // For non-negative numbers, it returns the integer square root if it is less than 255.
    // If the integer square root is 255 or greater, it returns 255.

    int num = atoi(argv[1]);
    int res;
    
    if (num < 0)
    {
        printf("Math Error!\n");
        return 0; // Return 0 indicating error
    }
    else
    {
        res = (int)sqrt(num);
        printf("%d\n", res);
        if (res < 255)
        {
            return res; // Return the result if it's within the valid range
        }
        else
        {
            return 255; // Return 255 if the result exceeds the valid range
        }
    }
}