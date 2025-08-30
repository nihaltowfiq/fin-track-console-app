#include <stdio.h>
#include <stdlib.h>

#include "./menu.h"

void print_header() {
    printf("\n============= FinTrack ==============\n");
    printf("   Your Personal Finance Tracker\n Budget Smart, Spend Wise, Save More  \n");
    printf("=====================================\n\n");
}

int main() {
    int choice;
    char current_user[USERNAME_MAX];
    int is_authenticated = 0;

    while (1) {

        if (!is_authenticated) {
            print_header();
            show_guest_menu();
        } else {
            show_user_menu();
        }

        printf("\nChoose option: ");
        scanf("%d", &choice);
        getchar();

        if (!is_authenticated) {
            handle_guest_choice(choice, &is_authenticated, current_user);
            if (choice == 3) break;
        } else {
            handle_user_choice(choice, &is_authenticated, current_user);
            if (choice == 9) break;
        }
    }
    return 0;
}
