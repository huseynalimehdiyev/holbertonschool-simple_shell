#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
<<<<<<< HEAD
#include <sys/types.h>
=======
>>>>>>> ed40bc41dee8e8c278c3b299df103caf49d832cb
#include <sys/wait.h>

extern char **environ;

static void print_prompt(void)
{
    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, "#cisfun$ ", 9);
}

static void print_error(const char *shell_name)
{
    dprintf(STDERR_FILENO, "%s: No such file or directory\n", shell_name);
}

int main(int argc, char **argv)
{
<<<<<<< HEAD
    char *line;
    size_t cap;
    ssize_t nread;
    pid_t pid;
    int status;
    char *args[2];

    (void)argc;

    line = NULL;
    cap = 0;
=======
    (void)argc;

    char *line = NULL;
    size_t cap = 0;
    ssize_t nread;
>>>>>>> ed40bc41dee8e8c278c3b299df103caf49d832cb

    while (1)
    {
        print_prompt();

        nread = getline(&line, &cap, stdin);
        if (nread == -1)
        {
<<<<<<< HEAD
=======
            /* Ctrl+D / EOF */
>>>>>>> ed40bc41dee8e8c278c3b299df103caf49d832cb
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

<<<<<<< HEAD
        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        if (line[0] == '\0')
            continue;

        pid = fork();
=======
        /* Remove trailing newline */
        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Skip empty line */
        if (line[0] == '\0')
            continue;

        pid_t pid = fork();
>>>>>>> ed40bc41dee8e8c278c3b299df103caf49d832cb
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
<<<<<<< HEAD
=======
            /* Child: execute single-word command, no args */
            char *args[2];
>>>>>>> ed40bc41dee8e8c278c3b299df103caf49d832cb
            args[0] = line;
            args[1] = NULL;

            execve(args[0], args, environ);

<<<<<<< HEAD
=======
            /* If execve fails */
>>>>>>> ed40bc41dee8e8c278c3b299df103caf49d832cb
            print_error(argv[0]);
            _exit(127);
        }
        else
        {
<<<<<<< HEAD
=======
            /* Parent: wait */
            int status;
>>>>>>> ed40bc41dee8e8c278c3b299df103caf49d832cb
            (void)waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}
