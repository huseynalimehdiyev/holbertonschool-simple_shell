#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/**
 * find_command - search command in PATH
 * @cmd: command name
 * Return: full path or NULL
 */
char *find_command(char *cmd)
{
    char *path, *path_copy, *dir;
    char full_path[1024];

    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (strdup(cmd));
        return (NULL);
    }

    path = getenv("PATH");
    if (!path)
        return (NULL);

    path_copy = strdup(path);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");

    while (dir)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (strdup(full_path));
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return (NULL);
}

/**
 * main - simple UNIX shell
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *argv[100];
    char *token;
    int i, status;
    pid_t pid;
    char *cmd_path;

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            printf(":) ");
            fflush(stdout);
        }

        nread = getline(&line, &len, stdin);

        /* Ctrl+D */
        if (nread == -1)
        {
            free(line);
            exit(0);
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Tokenize */
        i = 0;
        token = strtok(line, " \t");

        while (token && i < 99)
        {
            argv[i++] = token;
            token = strtok(NULL, " \t");
        }
        argv[i] = NULL;

        if (argv[0] == NULL)
            continue;

        /* 🔥 EXIT BUILTIN (NO FORK) */
        if (strcmp(argv[0], "exit") == 0)
        {
            free(line);
            return (0);
        }

        /* Find command BEFORE fork */
        cmd_path = find_command(argv[0]);

        if (!cmd_path)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            continue;
        }

        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            free(cmd_path);
            continue;
        }

        if (pid == 0)
        {
            execve(cmd_path, argv, environ);
            perror("execve");
            exit(1);
        }
        else
        {
            wait(&status);
        }

        free(cmd_path);
    }

    free(line);
    return (0);
}
