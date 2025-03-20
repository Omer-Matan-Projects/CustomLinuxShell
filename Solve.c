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
    // The return value of this program indicates the roots of a quadratic equation.
    // If 'a' is 0 it returns 0.
    // If the discriminant is positive, indicating two real roots, it returns 2.
    // If the discriminant is zero, indicating one real root, it returns 1.
    // If the discriminant is negative, indicating imaginary roots, it returns 255.
    
    // Convert command-line arguments to integers
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int c = atoi(argv[3]);
    int discriminant, root1, root2;
    
    // Check if 'a' is zero (not a quadratic equation)
    if (a == 0)
    {
        printf("This is not a quadratic equation (a cannot be zero)\n");
        return 0;
    }

    discriminant = b * b - 4 * a * c; // Calculate discriminant

    // Check the nature of the roots
    if (discriminant > 0)
    {
        // Roots are real and distinct
        root1 = (-b + (int)sqrt(discriminant)) / (2 * a);
        root2 = (-b - (int)sqrt(discriminant)) / (2 * a);
        printf("%d\n", root1);
        printf("%d\n", root2);
        return 2; // Return 2 for two real roots
    }
    else if (discriminant == 0)
    {
        // Roots are real and equal
        root1 = -b / (2 * a);
        printf("%d\n", root1);
        return 1; // Return 1 for one real root
    }
    else
    {
        // Roots are imaginary
        printf("No Sol\n");
        return 255; // Return 255 for imaginary roots
    }
}