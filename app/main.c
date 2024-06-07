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

      // exit condition check
      if ((match_command(input, "exit 0")) == 0) {
        exit_bool = true;
      } else if ((match_command(input, "echo")) == 0) {
        for (int i = 5; i < strlen(input); i++) {
          printf("%c", input[i]);
        }
        printf("\n");
      } else {
        printf("%s: command not found\n", input);
      }
    }
  }

  return 0;
}
