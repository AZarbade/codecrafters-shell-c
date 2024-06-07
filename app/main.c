#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

        if (match_command(inner_command, "echo") == 0) {
          printf("%s is a shell builtin\n", inner_command);
        } else if (match_command(inner_command, "exit") == 0) {
          printf("%s is a shell builtin\n", inner_command);
        } else if (match_command(inner_command, "type") == 0) {
          printf("%s is a shell builtin\n", inner_command);
        } else {
          printf("%s not found\n", inner_command);
        }

        strcpy(inner_command, "");
      } else {
        // command not found
        printf("%s: command not found\n", input);
      }
    }
  }

  return 0;
}

// struct dirent **namelist;
// int n;
//
// printf("Tokens:\n");
// for (int i = 0; i < tok_count; i++) {
//   printf("%s\n", tokens[i]);
// }
// for (int i = 0; i < tok_count; i++) {
//   n = scandir(tokens[i], &namelist, NULL, alphasort);
//   while (n--) {
//     printf("%s\n", namelist[n]->d_name);
//     free(namelist[n]);
//   }
//   free(namelist);
// }
