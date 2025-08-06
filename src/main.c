#include <stdio.h>
#include <stdlib.h>

int main() {
  int choice;

  printf("==== Welcome to FinTrack ====\n");

  while (1) {
    printf("\n1. Login\n2. Sign Up\n3. Exit\n");
    printf("Choose option: ");
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        // call login()
        break;
      case 2:
        // call signup()
        break;
      case 3:
        printf("Exiting FinTrack...\n");
        exit(0);
        break;

      default:
        // call
        printf("Invalid option!\n");
    }
  }

  return 0;
}
