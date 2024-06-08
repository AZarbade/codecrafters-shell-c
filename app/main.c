#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *parse_env() {
  // parse env. variables
  char *path = getenv("PATH");
  if (path != NULL) {
    char *pathCopy = strdup(path);
    if (pathCopy == NULL) {
      printf("ERROR: failed to allocated memory for Path\n");
      return NULL;
    } else {
      return pathCopy;
    }
  } else {
    printf("PATH environment variable is not set.\n");
    return NULL;
  }
}

void find_exec(char *command, char *path_tokens[], int tok_count) {
  bool found = false;
  char buf[1024];
  for (int i = 0; i < tok_count; i++) {
    int len = snprintf(buf, sizeof(buf), "%s/%s", path_tokens[i], command);
    if (len >= 0 && access(buf, F_OK) == 0) {
      printf("%s is %s\n", command, buf);
      found = true;
      break;

    } else {
      continue;
    }
  }

  if (!found) {
    printf("%s: command not found\n", command);
  }
}

void run_exec(char *inner_command, char *path_tokens[], int tok_count) {
  bool found = false;
  char buf[1024];
  int arg_count = 0;
  char *argv[100];

  argv[arg_count++] = inner_command;
  char *token = strtok(NULL, " ");
  while (token != NULL) {
    argv[arg_count++] = token;
    token = strtok(NULL, " ");
  }
  argv[arg_count] = NULL;

  for (int i = 0; i < tok_count; i++) {
    int len =
        snprintf(buf, sizeof(buf), "%s/%s", path_tokens[i], inner_command);
    if (len >= 0 && access(buf, F_OK) == 0) {
      found = true;
      execvp(buf, argv);
      break;
    }
  }

  if (!found) {
    printf("%s: command not found\n", inner_command);
  }
}

int main() {
  char *path = parse_env();

  char *pathCopy = strdup(path);
  char *path_tokens[100];
  int tok_count = 0;

  char *path_token = strtok(pathCopy, ":");
  while (path_token != NULL) {
    path_tokens[tok_count++] = path_token;
    path_token = strtok(NULL, ":");
  }

  free(pathCopy);

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

      // TODO: frick you space!!
      //                        v here figure out a way to include '0'
      if ((strcmp(command, "exit")) == 0) {
        // COMMAND: exit 0
        exit_bool = true;

      } else if ((strcmp(command, "echo")) == 0) {
        // COMMAND: echo
        token = strtok(NULL, " ");
        if (token != NULL) {
          printf("%s\n", token);
        }

      } else if ((strcmp(command, "type") == 0)) {
        // COMMAND: type
        token = strtok(NULL, " ");

        // find executable
        find_exec(token, path_tokens, tok_count);

      } else {
        run_exec(input, path_tokens, tok_count);
        // command not found
        printf("%s: command not found\n", input);
      }
    }
  }

  return 0;
}
