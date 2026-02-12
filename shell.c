#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
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
    char *line;
    size_t cap;
    ssize_t nread;
    pid_t pid;
    int status;
    char *args[2];

    (void)argc;

    line = NULL;
    cap = 0;

    while (1)
    {
        print_prompt();

        nread = getline(&line, &cap, stdin);
        if (nread == -1)
        {
            if (isatty(STDIN_FILENO))
                write(STDOUT_FILENO, "\n", 1);
            break;
        }

        if (nread > 0 && line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        if (line[0] == '\0')
            continue;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            args[0] = line;
            args[1] = NULL;

            execve(args[0], args, environ);

            print_error(argv[0]);
            _exit(127);
        }
        else
        {
            (void)waitpid(pid, &status, 0);
        }
    }

    free(line);
    return 0;
}
