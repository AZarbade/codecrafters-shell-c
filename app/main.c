#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// TODO: use struct to define path
// TODO: use struct to define user inputs?

char *parse_env() {
  // parse env. variables
  char *sysPath = getenv("PATH");
  char *pathCopy = strdup(sysPath);

  if (pathCopy == NULL) {
    fprintf(stderr, "Failed to get global_path.\n");
  }
  return pathCopy;
}

void find_exec(char *exec) {
  // find executables for in given PATHS
  char *path = parse_env();
  char *path_tokens[100];
  int path_count = 0;

  char *path_token = strtok(path, ":");
  while (path_token != NULL) {
    path_tokens[path_count++] = path_token;
    path_token = strtok(NULL, ":");
  }

  bool found = false;
  char buf[100];

  for (int i = 0; i < path_count; i++) {
    snprintf(buf, sizeof(buf), "%s/%s", path_tokens[i], exec);
    if (access(buf, F_OK) == 0) {
      printf("%s is %s\n", exec, buf);
      found = true;
      break;
    }
  }

  if (!found) {
    printf("%s: command not found\n", exec);
  }

  free(path);
}

void run_exec(char *input) {
  // run the given executable in given PATHS
  char *path = parse_env();
  char *path_tokens[100];
  int path_count = 0;

  char *path_token = strtok(path, ":");
  while (path_token != NULL) {
    path_tokens[path_count++] = path_token;
    path_token = strtok(NULL, ":");
  }

  char *token = strtok(input, " ");
  char *command = token;
  char *arguments = strtok(NULL, "");
  char *args[100];
  int args_count = 0;

  char *args_token = strtok(arguments, " ");
  while (args_token != NULL) {
    args[args_count++] = args_token;
    args_token = strtok(NULL, " ");
  }
  args[args_count] = NULL;

  // bool found = false;
  // char buf[100];
  //
  // for (int i = 0; i < path_count; i++) {
  //   snprintf(buf, sizeof(buf), "%s/%s", path_tokens[i], command);
  //   if ((access(buf, F_OK | X_OK) == 0)) {
  //     // execvp(buf, args); // searches PATH for executables then executes.
  //     execv(buf, args); // have to provide full PATH of executables.
  //     found = true;
  //     break;
  //   }
  // }
  //
  // if (!found) {
  //   printf("%s: command not found\n", command);
  // }
  //
  // free(path);
}

int main() {
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
          find_exec(exec);
        }
      }

    } else {
      run_exec(command);
    }
  }

  return 0;
}
