#include <stdio.h>      // Required for printf
#include <stdlib.h>     // Required for exit, EXIT_FAILURE, malloc, free
#include <fcntl.h>      // Required for open
#include <errno.h>      // Required for error handling
#include <sys/types.h>  // Required for basic data types and structures used in system calls and POSIX standards
#include <sys/stat.h>   // Required for mkdir and file permission constants
#include <unistd.h>     // Required for write, close, and usleep
#include <sys/wait.h>   // Required for wait and waitpid
#include <string.h>

// Global variables
int fd_file;
int counter;

// Function declarations
void InitializeCounter();
void Document(char** args);
int CreateNewProcessAndExec(char* path, char** args);
void ErrorCleanUp(char* errorMsg, int fd_file, int free_flag, char** args);

// Function to initialize the counter based on existing commands in the file
void InitializeCounter()
{
    char buffer[1024];
    int rbytes, lines = 0;
    
    lseek(fd_file, 0, SEEK_SET); // Reset file offset to start

    // Read and count lines in the file
    while ((rbytes = read(fd_file, buffer, sizeof(buffer))) > 0)
    {
        for (int i = 0; i < rbytes; i++)
        {
            if (buffer[i] == '\n')
                lines++;
        }
    }

    // Check for read errors
    if (rbytes == -1)
    {
        ErrorCleanUp("read failed", fd_file, 0, NULL);
    }

    counter = lines + 1; // Set counter to the number of lines + 1 for the next command
}

// Function to log commands to file
void Document(char** args)
{
    int i = 0, wbytes;
    char command_doc[256];

    snprintf(command_doc, sizeof(command_doc), "   %d.   ", counter); // Prefix with command number

    if (strcmp(args[0], "History") == 0)
    {
        snprintf(command_doc + strlen(command_doc), sizeof(command_doc) - strlen(command_doc), "%s ", args[0]); // Append each argument to command_doc with a space
    }
    else
    {
        // Build the command string based on args
        while (args[i] != NULL)
        {
            snprintf(command_doc + strlen(command_doc), sizeof(command_doc) - strlen(command_doc), "%s ", args[i]); // Append each argument to command_doc with a space
            i++;
        }
    }

    snprintf(command_doc + strlen(command_doc), sizeof(command_doc) - strlen(command_doc), "\n"); // Append a newline to command_doc

    // Write command_doc to the file
    if ((wbytes = write(fd_file, command_doc, strlen(command_doc))) == -1)
    {
        ErrorCleanUp("write failed", fd_file, 1, args);
    }
    else if (wbytes != strlen(command_doc)) // Handle partial write scenario
    {
        ErrorCleanUp("partial write", fd_file, 1, args);
    }

    counter++; // Increment the command counter for the next command
}

// Function to create a new process and execute a command. Returns the child process PID
int CreateNewProcessAndExec(char* path, char** args)
{
    int pid = fork(); // Create a new process
    if (pid == -1)
    {
        ErrorCleanUp("fork failed", fd_file, 1, args);
    }
    else if (pid == 0) // Child process
    {
        if (execv(path, args) == -1) // Execute the command
        {
            ErrorCleanUp("execv failed", fd_file, 1, args);
        }
    }
    
    return pid; // Parent process
}

// Function to handle errors and clean up resources
void ErrorCleanUp(char* errorMsg, int fd_file, int free_flag, char** args)
{
    perror(errorMsg); // Print the error message

    if (free_flag == 1) // If free_flag is set
    {
        free(args); // Free the allocated memory for args
    }

    if (fd_file != 0)
    {
        if (close(fd_file) == -1) // Close the file descriptor
        {
            perror("close failed");
        }
    }

    exit(EXIT_FAILURE); // Exit the program with failure status
}

int main(int argc, char* argv[])
{
    int pid, status;
    char command[256];
    char path[256];

    if ((fd_file = open("Commands/Math/Math_Commands.txt", O_RDWR, 0664)) == -1)
    {
        ErrorCleanUp("open failed", fd_file, 0, NULL); // Handle file open failure
    }

    // Initialize the counter based on existing commands
    InitializeCounter();

    while (1)
    {
        printf("MathShell> "); // Display the prompt

        // Read the command from the user
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            perror("fgets failed"); // Handle error if fgets fails
            continue;
        }

        // Remove the newline character from the input
        command[strcspn(command, "\n")] = '\0'; // Replace newline with null terminator

        // Check if the command is empty
        if (command[0] == '\0')
        {
            continue; // Skip the empty command
        }

        int arg_count = 0; // Count the number of tokens
        char** args = NULL; // Args array for execv
        char** temp;
        char* token = strtok(command, " ");

        while (token != NULL)
        {
            // Allocate memory for args dynamically
            temp = realloc(args, (arg_count + 2) * sizeof(char*));
            if (temp == NULL)
            {
                if (arg_count == 0) // No memory allocated yet
                {
                    ErrorCleanUp("realloc failed", fd_file, 0, NULL);
                }
                else // Memory already allocated
                {
                    ErrorCleanUp("realloc failed", fd_file, 1, args);
                }
            }
            args = temp;

            args[arg_count++] = token; // Store the token in args

            token = strtok(NULL, " "); // Get the next token
        }

        args[arg_count] = NULL; // Add NULL as the last argument

        // Handle supported commands
        if (strcmp(args[0], "Sqrt") == 0)
        {
            Document(args); // Log the command
            
            // Check number of arguments
            if (arg_count != 2)
            {
                ErrorCleanUp("More/less than 1 argument were entered", fd_file, 1, args);
            }
            pid = CreateNewProcessAndExec("./Sqrt", args); // Execute Sqrt command
            free(args); // Free args memory
        }
        else if (strcmp(args[0], "Power") == 0)
        {
            Document(args); // Log the command

            // Check number of arguments
            if (arg_count != 3)
            {
                ErrorCleanUp("More/less than 2 argument were entered", fd_file, 1, args);
            }
            pid = CreateNewProcessAndExec("./Power", args); // Execute Power command
            free(args); // Free args memory
        }
        else if (strcmp(args[0], "Solve") == 0)
        {
            Document(args); // Log the command

            // Check number of arguments
            if (arg_count != 4)
            {
                ErrorCleanUp("More/less than 3 argument were entered", fd_file, 1, args);
            }
            pid = CreateNewProcessAndExec("./Solve", args); // Execute Solve command
            free(args); // Free args memory
        }
        else if (strcmp(args[0], "History") == 0)
        {
            Document(args); // Log the command

            // Check arguments
            if (arg_count != 1)
            {
                ErrorCleanUp("This command does not accept arguments", fd_file, 1, args);
            }

            // Add space for "Math" argument and NULL terminator
            temp = realloc(args, (arg_count + 2) * sizeof(char*));
            if (temp == NULL)
            {
                ErrorCleanUp("realloc failed", fd_file, 1, args); // Handle realloc failure
            }
            args = temp;
            
            // Set "Math" argument and NULL terminator
            args[1] = "Math";
            args[2] = NULL;

            pid = CreateNewProcessAndExec("./History", args); // Execute History command
            free(args); // Free args memory
        }
        else if (strcmp(args[0], "Cls") == 0)
        {
            Document(args); // Log the command
            
            // Check arguments
            if (arg_count != 1)
            {
                ErrorCleanUp("This command does not accept arguments", fd_file, 1, args);
            }
            free(args); // Free args memory
            
            if (close(fd_file) == -1) // Close the file descriptor
            {
                perror("close failed");
                exit(EXIT_FAILURE);
            }

            exit(0); // Exit the program
        }
        else // Unsupported command
        {
            Document(args); // Log the command

            printf("Not Supported\n");
            free(args); // Free args memory
            continue;
        }

        // Parent process waits for the current child to finish
        if (waitpid(pid, &status, 0) == -1)
        {
            ErrorCleanUp("waitpid failed", fd_file, 1, args); // Handle waitpid failure
        }
        // if status = 0, the command executed successfully
    }

    return 0;
}