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
int math_flag = 0, logic_flag = 0, string_flag = 0; // Flags to indicate if specific directories were created

// Functions declarations
void CreateDir(char* dirName);
int CreateDirWithFile(char* shellName);
int CreateNewProcessAndExec(char* path, char** args);
void RemoveDir(char* dirName);
void RemoveDirWithFile(char* shellName);
void RemoveAll(int math_flag, int logic_flag, int string_flag);
void ErrorCleanUp(char* errorMsg, int free_flag, char** args);

// Function to create a directory with the given name
void CreateDir(char* dirName)
{
    if (mkdir(dirName, 0755) == -1) // Create directory with read/write/execute permissions for owner, and read/execute permissions for group and others
    {
        ErrorCleanUp("mkdir failed", 0, NULL);
    }
}

// Function to create a directory for a specific shell and create a file inside it. Returns 1 if successful (used as a flag)
int CreateDirWithFile(char* shellName)
{
    int fd_file;
    char path[256];
    char fileName[256];
    
    snprintf(path, sizeof(path), "Commands/%s", shellName); // Construct the directory path
    snprintf(fileName, sizeof(fileName), "Commands/%s/%s_Commands.txt", shellName, shellName); // Construct the file path
    
    CreateDir(path); // Create the directory

    if ((fd_file = open(fileName, O_WRONLY | O_CREAT | O_EXCL, 0664)) == -1) // Create and open the file with read/write permissions for owner and read permissions for group and others. fail if the file already exists
    {
        if (errno != EEXIST) // If the error is not that the file already exists
        {
            ErrorCleanUp("open failed", 0, NULL);
        }
        // If the file already exists, do nothing (no error cleanup needed)
    }

    if (close(fd_file) == -1) // Close the file descriptor
    {
        ErrorCleanUp("close failed", 0, NULL);
    }

    return 1;
}

// Function to create a new process and execute a command. Returns the child process PID
int CreateNewProcessAndExec(char* path, char** args)
{
    int pid = fork(); // Create a new process
    if (pid == -1)
    {
        if (args == NULL)
        {
            ErrorCleanUp("fork failed", 0, NULL);
        }
        else
        {
            ErrorCleanUp("fork failed", 1, args);
        }
    }
    else if (pid == 0) // Child process
    {
        if (execv(path, args) == -1) // Execute the command
        {
            if (args == NULL)
            {
                ErrorCleanUp("execv failed", 0, NULL);
            }
            else
            {
                ErrorCleanUp("execv failed", 1, args);
            }
        }
    }
    
    return pid; // Parent process
}

// Function to remove a directory with the given name
void RemoveDir(char* dirName)
{
    if (rmdir(dirName) == -1) // Remove the directory
    {
        ErrorCleanUp("rmdir failed", 0, NULL);
    }
}

// Function to remove a directory with a file in it
void RemoveDirWithFile(char* shellName)
{
    char path[256];
    char fileName[256];
    
    snprintf(path, sizeof(path), "Commands/%s", shellName); // Construct the directory path
    snprintf(fileName, sizeof(fileName), "Commands/%s/%s_Commands.txt", shellName, shellName); // Construct the file path
    
    if (remove(fileName) != 0)
    {
        ErrorCleanUp("remove failed", 0, NULL);
    }
    RemoveDir(path);
}

// Function to remove directories and files associated with specific flags and cleans up the Commands directory
void RemoveAll(int math_flag, int logic_flag, int string_flag)
{
    if (math_flag) // If the Math directory was created
    {
        RemoveDirWithFile("Math"); // Remove the Math directory and file
    }
    if (logic_flag) // If the Logic directory was created
    {
        RemoveDirWithFile("Logic"); // Remove the Logic directory and file
    }
    if (string_flag) // If the String directory was created
    {
        RemoveDirWithFile("String"); // Remove the String directory and file
    }
    RemoveDir("Commands"); // Remove the Commands directory
}

// Function to handle errors and clean up resources
void ErrorCleanUp(char* errorMsg, int free_flag, char** args)
{
    perror(errorMsg); // Print the error message

    if (free_flag == 1) // If free_flag is set
    {
        free(args); // Free the allocated memory for args
    }

    RemoveAll(math_flag, logic_flag, string_flag);

    exit(EXIT_FAILURE); // Exit the program with failure status
}

int main(int argc, char* argv[])
{
    int pid, status;
    char command[256]; // Buffer for the input command
    char path[261]; // Buffer for the executable file path (256 for the command name, 5 for "/bin/")

    CreateDir("Commands"); // Create the Commands directory

    while (1)
    {
        printf("StandShell> "); // Display the prompt

        // Read the command from the user
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            perror("fgets failed"); // Handle error if fgets fails
            continue;
        }

        // Remove the newline character from the input
        command[strcspn(command, "\n")] = '\0'; // Find the position of the newline character and replace it with a null terminator

        // Check if the command is empty
        if (command[0] == '\0')
        {
            continue; // Skip the empty command
        }

        if (strcmp(command, "Math") == 0) // If the command is "Math"
        {
            if (math_flag == 0) // Create the Math directory and file (if it doesn't already exist)
            {
                math_flag = CreateDirWithFile("Math");
            }
            pid = CreateNewProcessAndExec("./Math_shell", NULL); // Create a new process and execute the Math shell
        }
        else if (strcmp(command, "Logic") == 0) // If the command is "Logic"
        {
            if (logic_flag == 0) // Create the Logic directory and file (if it doesn't already exist)
            {
                logic_flag = CreateDirWithFile("Logic");
            }
            pid = CreateNewProcessAndExec("./Logic_shell", NULL); // Create a new process and execute the Logic shell
        }
        else if (strcmp(command, "String") == 0) // If the command is "String"
        {
            if (string_flag == 0) // Create the String directory and file (if it doesn't already exist)
            {
                string_flag = CreateDirWithFile("String");
            }
            pid = CreateNewProcessAndExec("./String_shell", NULL); // Create a new process and execute the String shell
        }
        else if (strcmp(command, "exit") == 0) // If the command is "exit"
        {
            printf("Goodbye\n");
            RemoveAll(math_flag, logic_flag, string_flag);
            exit(0); // Closing the process
        }
        else // If the command is not recognized (Math/Logic/String/exit)
        {
            int arg_count = 0; // Count the number of tokens
            char** args = NULL; // args array for execv
            char* token = strtok(command, " ");

            while (token != NULL)
            {
                // Check if more than 3 arguments were entered
                if (arg_count > 3)
                {
                    ErrorCleanUp("More than 3 arguments were entered", 1, args);
                }

                // Allocate memory for args dynamically
                char** temp = realloc(args, (arg_count + 2) * sizeof(char*));
                if (temp == NULL)
                {
                    if (arg_count == 0) // No memory allocated yet
                    {
                        ErrorCleanUp("realloc failed", 0, NULL);
                    }
                    else // Memory already allocated
                    {
                        ErrorCleanUp("realloc failed", 1, args);
                    }
                }
                args = temp;

                args[arg_count++] = token; // Store the token in args

                token = strtok(NULL, " "); // Get the next token (NULL means continue tokenizing the same string from a previous call)
            }

            args[arg_count] = NULL; // Add NULL as the last argument

            snprintf(path, sizeof(path), "/bin/%s", args[0]); // Create the path for the executable file

            // Check if the command exists in /bin
            if (access(path, X_OK) == -1)
            {
                printf("Not Supported\n");
                free(args); // Free the allocated memory for args
                continue;
            }

            pid = CreateNewProcessAndExec(path, args); // Create a new process and execute the command

            free(args); // Free the allocated memory for args
        }

        // Parent process waits for the current child to finish
        if (waitpid(pid, &status, 0) == -1)
        {
            ErrorCleanUp("waitpid failed", 0, NULL);
        }
    }

    return 0;
}