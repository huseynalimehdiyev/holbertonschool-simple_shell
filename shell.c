#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
    (void)argc;

    char *line = NULL;
    size_t cap = 0;
    ssize_t nread;

    while (1)
    {
        print_prompt();

        nread = getline(&line, &cap, stdin);
        if (nread == -1)
        {
            /* Ctrl+D / EOF */
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        /* Remove trailing newline */
        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Skip empty line */
        if (line[0] == '\0')
            continue;

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            /* Child: execute single-word command, no args */
            char *args[2];
            args[0] = line;
            args[1] = NULL;

            execve(args[0], args, environ);

            /* If execve fails */
            print_error(argv[0]);
            _exit(127);
        }
        else
        {
            /* Parent: wait */
            int status;
            (void)waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}
