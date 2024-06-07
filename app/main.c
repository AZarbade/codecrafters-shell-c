#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main() {
  bool exit_bool = false;

  while (!exit_bool) {
    printf("$ ");
    fflush(stdout);
    char input[100];

    if (fgets(input, 100, stdin) != NULL) {
      // input sanitization
      input[strlen(input) - 1] = '\0';

      // exit condition check
      if (strcmp(input, "exit 0") == 0) {
        exit_bool = true;
      } else {
        printf("%s: command not found\n", input);
      }
    }
  }

  return 0;
}
