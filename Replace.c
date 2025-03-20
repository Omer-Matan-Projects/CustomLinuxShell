#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

// This program takes an original string, a word, and a position from the command-line arguments.
// It inserts the word into the original string at the specified position, replacing characters in the original string.
// The program then prints the modified string.

int main(int argc, char* argv[])
{
    char* original_str = argv[1];
    char* word = argv[2];
    int position = atoi(argv[3]);

    int original_len = strlen(original_str); // Length of the original string
    int word_len = strlen(word); // Length of the word to insert
   
    // Check if the position is valid
    if (position <= 0 || position > original_len) 
    {
        printf("Invalid position: %d\n", position);
        exit(EXIT_FAILURE);
    }

    position--; // Adjust position for 0-based index
    
    int end_index = position + word_len; // Calculate the end index for replacement

    // Replace characters in the original string
    for (int i = position; i < end_index; ++i)
    {
        if (i < original_len)
        {
            original_str[i] = word[i - position]; // Replace character in original string with character from word
        }
        else
        {
            break; // Stop if we exceed the original string length
        }
    }

    printf("%s\n", original_str); // Print the modified string

    return 0; // Exit the program with success status
}