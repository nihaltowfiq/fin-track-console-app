#include "../include/auth.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int choice;
  char username[USERNAME_MAX], password[PASSWORD_MAX];

  while (1) {
    printf("\n==== FinTrack Auth ====\n");
    printf("1. Sign Up\n");
    printf("2. Login\n");
    printf("3. Exit\n");
    printf("Choose: ");
    scanf("%d", &choice);
    getchar(); // clear newline

    if (choice == 3) {
      printf("Goodbye!\n");
      break;
    }

    printf("Username: ");
    scanf("%31s", username);
    printf("Password: ");
    scanf("%63s", password);

    if (choice == 1) {
      if (signup(username, password))
        printf("✅ Signup successful!\n");
      else
        printf("❌ Signup failed! Username may already exist.\n");
    } else if (choice == 2) {
      if (login(username, password))
        printf("✅ Login successful! Welcome %s\n", username);
      else
        printf("❌ Login failed! Incorrect credentials.\n");
    } else {
      printf("Invalid option.\n");
    }
  }
  return 0;
}
