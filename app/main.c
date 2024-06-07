#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main() {
  bool run = true;

  while (run) {
    printf("$ ");
    fflush(stdout);
    char input[100];
    if (fgets(input, 100, stdin) != NULL) {
      input[strlen(input) - 1] = '\0';
      printf("%s: command not found\n", input);
    }
  }

  return 0;
}
