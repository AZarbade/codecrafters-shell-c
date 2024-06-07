#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int match_command(char *input, char *command) {
  for (int i = 0; i < strlen(command); i++) {
    if (input[i] != command[i]) {
      return 1;
    }
  }

  return 0;
}

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

void find_exec(char *inner_command, char *tokens[], int tok_count) {
  bool found = false;
  char buf[1024];
  for (int i = 0; i < tok_count; i++) {
    int len = snprintf(buf, sizeof(buf), "%s/%s", tokens[i], inner_command);
    if (len >= 0 && access(buf, F_OK) == 0) {
      printf("%s is %s\n", inner_command, buf);
      found = true;
      break;

    } else {
      continue;
    }
  }

  if (!found) {
    printf("%s: command not found\n", inner_command);
  }
}

void run_exec(char *inner_command, char *tokens[], int tok_count) {
  bool found = false;
  char buf[1024];
  for (int i = 0; i < tok_count; i++) {
    int len = snprintf(buf, sizeof(buf), "%s/%s", tokens[i], inner_command);
    if (len >= 0 && access(buf, F_OK) == 0) {
      found = true;

      char *args[] = {NULL};
      execv(buf, args);

      break;

    } else {
      continue;
    }
  }

  if (!found) {
    printf("%s: command not found\n", inner_command);
  }
}

int main() {
  char *path = parse_env();

  char *pathCopy = strdup(path);
  char *tokens[100];
  int tok_count = 0;

  char *token = strtok(pathCopy, ":");
  while (token != NULL) {
    tokens[tok_count++] = token;
    token = strtok(NULL, ":");
  }

  printf("Tokens:\n");
  for (int i = 0; i < tok_count; i++) {
    printf("%s\n", tokens[i]);
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

      if ((match_command(input, "exit 0")) == 0) {
        // exit condition check
        exit_bool = true;

      } else if ((match_command(input, "echo")) == 0) {
        // command: echo
        // TODO: get rid of hardcoded command count
        //           v
        for (int i = 5; i < strlen(input); i++) {
          printf("%c", input[i]);
        }
        printf("\n");

      } else if ((match_command(input, "type") == 0)) {
        // extract inner_command
        char inner_command[100];
        for (int i = 5; input[i] != '\0'; i++) {
          char temp[2] = {input[i], '\0'};
          strcat(inner_command, temp);
        }

        // find executable
        find_exec(inner_command, tokens, tok_count);

        // set inner_command to null
        strcpy(inner_command, "");

      } else {
        run_exec(input, tokens, tok_count);
        // command not found
        printf("%s: command not found\n", input);
      }
    }
  }

  return 0;
}
