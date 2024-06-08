#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *parse_env() {
  // parse env. variables
  char *sysPath = getenv("PATH");
  char *pathCopy = strdup(sysPath);

  if (pathCopy == NULL) {
    fprintf(stderr, "Failed to get path.\n");
  }
  return pathCopy;
}

void find_exec(char *command) {
  // find executables for in given PATHS
  printf("NOT IMPLEMENTED!\n");
}

void run_exec(char *input) {
  // run the given executable in given PATHS
  printf("NOT IMPLEMENTED!\n");
}

int main() {
  char *path = parse_env();
  printf("Path parseed\n");
  char *path_tokens[100];
  int path_count = 0;

  char *path_token = strtok(path, ":");
  while (path_token != NULL) {
    path_tokens[path_count++] = path_token;
    path_token = strtok(NULL, ":");
  }
  printf("%s\n", path_tokens[3]);
  free(path); // TODO: is this vaild?

  // main loop
  bool exit = false;
  while (!exit) {
    printf("$ ");
    fflush(stdout);
    char input[100];

    // input Non-Null check
    if (fgets(input, 100, stdin) != NULL) {
      // input sanitization
      input[strlen(input) - 1] = '\0';
    }

    // Seperating command and its arguments
    char *token = strtok(input, " ");
    char *command = token;
    char *arguments = strtok(NULL, "");
    printf("command: %s\n", command);
    printf("args: %s\n", arguments);
    printf("----------\n");

    if ((strcmp(command, "exit") == 0)) {
      exit = true;

    } else if ((strcmp(command, "echo") == 0)) {
      printf("%s\n", arguments);

    } else if ((strcmp(command, "type") == 0)) {
      if (arguments == NULL) {
        printf("- usage: type <command>\n");

      } else {
        char *exec = strtok(arguments, " ");
        // Check for builtin commands
        if (exec != NULL && (strcmp(exec, "echo") == 0)) {
          printf("%s is a shell builtin\n", exec);

        } else if (exec != NULL && (strcmp(exec, "exit") == 0)) {
          printf("%s is a shell builtin\n", exec);

        } else if (exec != NULL && (strcmp(exec, "type") == 0)) {
          printf("%s is a shell builtin\n", exec);

        } else {
          // Find exec in Path
          find_exec(exec);
        }
      }

    } else {
      run_exec(command);
    }
  }

  return 0;
}
