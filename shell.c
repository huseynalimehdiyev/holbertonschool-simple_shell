#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

/**
 * _getenv - get environment variable value
 * @name: variable name
 * Return: pointer to value or NULL
 */
char *_getenv(const char *name)
{
    int i = 0;
    size_t len = strlen(name);

    while (environ[i])
    {
        if (strncmp(environ[i], name, len) == 0 &&
            environ[i][len] == '=')
        {
            return (environ[i] + len + 1);
        }
        i++;
    }

    return (NULL);
}

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

    path = _getenv("PATH");
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
 * Return: exit status
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    char *argv[100];
    char *token;
    int i, status;
    int last_status = 0;
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

        if (nread == -1)
        {
            free(line);
            exit(last_status);
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

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

        /* EXIT built-in */
        if (strcmp(argv[0], "exit") == 0)
        {
            free(line);
            exit(last_status);
        }

        /* ENV built-in */
        if (strcmp(argv[0], "env") == 0)
        {
            int j = 0;
            while (environ[j])
            {
                printf("%s\n", environ[j]);
                j++;
            }
            last_status = 0;
            continue;
        }

        /* Find command BEFORE fork */
        cmd_path = find_command(argv[0]);

        if (!cmd_path)
        {
            fprintf(stderr, "%s: command not found\n", argv[0]);
            last_status = 127;
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
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
        }

        free(cmd_path);
    }

    free(line);
    return (last_status);
}
