#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int match_command(char *input, char *command) {
  for (int i = 0; i < strlen(command); i++) {
    if (input[i] != command[i]) {
      return 1;
    }
  }

  return 0;
}

int main() {
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
        // if echo then echo
        // if exit then exit
        // else nonexistent!

        // extract command
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
