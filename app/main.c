#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *parse_env() {
  // parse env. variables
  char *path = getenv("PATH");
  if (path == NULL) {
    fprintf(stderr, "PATH environment variable is not set.\n");
    return NULL;
  }

  char *pathCopy = strdup(path);
  if (pathCopy == NULL) {
    fprintf(stderr, "Failed to allocate memory for Path.\n");
  }
  return pathCopy;
}

void find_exec(char *command, char *path_tokens[], int tok_count) {
  // find executables for in given PATHS
  bool found = false;
  char buf[1024];

  for (int i = 0; i < tok_count; i++) {
    snprintf(buf, sizeof(buf), "%s/%s", path_tokens[i], command);
    if (access(buf, F_OK) == 0) {
      printf("%s is %s\n", command, buf);
      found = true;
      break;
    }
  }

  if (!found) {
    printf("%s: not found\n", command);
  }
}

void run_exec(char *inner_command, char *path_tokens[], int tok_count) {
  // run the given executable in given PATHS
  bool found = false;
  char buf[1024];
  int arg_count = 0;
  char *argv[100];

  char *token = strtok(inner_command, " ");
  while (token != NULL) {
    argv[arg_count++] = token;
    token = strtok(NULL, " ");
  }
  argv[arg_count] = NULL;

  for (int i = 0; i < tok_count; i++) {
    snprintf(buf, sizeof(buf), "%s/%s", path_tokens[i], inner_command);
    if (access(buf, F_OK) == 0) {
      execvp(buf, argv);
      found = true;
    }
  }

  if (!found) {
    printf("%s: command not found\n", inner_command);
  }
}

int main() {
  char *path = parse_env();

  char *pathCopy = strdup(path);
  char *path_tokens[1024];
  int tok_count = 0;

  char *path_token = strtok(pathCopy, ":");
  while (path_token != NULL) {
    path_tokens[tok_count++] = path_token;
    path_token = strtok(NULL, ":");
  }

  free(path);

  // main loop
  bool exit_bool = false;

  while (!exit_bool) {
    printf("$ ");
    fflush(stdout);

    char input[100];
    // input Non-Null check
    if (fgets(input, 100, stdin) != NULL) {
      // input sanitization
      input[strlen(input) - 1] = '\0';

      char *token = strtok(input, " ");
      char *command = token;

      // TODO: frick you space!! This will still pass the tests, but it's
      // wrong.
      //                        v here figure out a way to include '0'
      if ((strcmp(command, "exit")) == 0) {
        // COMMAND: exit 0
        exit_bool = true;

      } else if ((strcmp(command, "echo")) == 0) {
        // COMMAND: echo
        char *remainingInput = strtok(NULL, "");
        if (remainingInput != NULL) {
          printf("%s\n", remainingInput);
        } else {
          printf("\n");
        }

      } else if ((strcmp(command, "PATH")) == 0) {
        // COMMAND: PATH
        for (int i = 0; i < tok_count; i++) {
          printf("%s\n", path_tokens[i]);
        }

      } else if ((strcmp(command, "type") == 0)) {
        // COMMAND: type
        token = strtok(NULL, " ");

        if (token != NULL && (strcmp(token, "echo") == 0)) {
          printf("%s is a shell builtin\n", token);
        } else if (token != NULL && (strcmp(token, "exit") == 0)) {
          printf("%s is a shell builtin\n", token);
        } else if (token != NULL && (strcmp(token, "type") == 0)) {
          printf("%s is a shell builtin\n", token);
        } else {
          // find executable
          find_exec(token, path_tokens, tok_count);
        }

      } else {
        run_exec(input, path_tokens, tok_count);
      }
    }
  }

  return 0;
}
