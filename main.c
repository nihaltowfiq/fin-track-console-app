#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./auth.h"
#include "./config.h"
#include "./transaction.h"

int main() {
    int choice;
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    int logged_in = 0; // track login state
    char current_user[USERNAME_MAX];

    while (1) {
        printf("\n==== FinTrack ====\n");
        if (!logged_in) {
            printf("1. Login\n");
            printf("2. Sign Up\n");
            printf("3. Exit\n");
        } else {
            printf("1. Update Monthly Income\n");
            printf("2. Update Monthly Budget\n");
            printf("3. Add Transaction\n");
            printf("4. View Transactions by Month\n");

            // logout and exit these two option will be last options and implemented at last
            //printf("1. Logout\n");
            //printf("2. Exit\n");
        }
        printf("Choose: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        if (!logged_in) {
            if (choice == 3) {
                printf("Goodbye!\n");
                break;
            }

            printf("Username: ");
            scanf("%31s", username);
            printf("Password: ");
            scanf("%63s", password);

            if (choice == 1) {
                if (login(username, password)) {
                    printf("Login successful! Welcome %s\n", username);
                    logged_in = 1;
                    strcpy(current_user, username);
                } else {
                    printf("Login failed! Incorrect credentials.\n");
                }

            } else if (choice == 2) {
                double income, budget;
                printf("Enter Monthly Income: ");
                scanf("%lf", &income);
                printf("Enter Monthly Budget: ");
                scanf("%lf", &budget);

                if (signup(username, password, income, budget))
                    printf("Signup successful!\n");
                else
                    printf("Signup failed! Username may already exist.\n");
            } else {
                printf("Invalid option.\n");
            }
        } else {

            // If logged in

            // logout and exit these two option will be last options and implemented at last
            //if (choice == 1) {
            //printf("Logged out.\n");
            //logged_in = 0;
            //} else if (choice == 2) {
            //printf("Goodbye!\n");
            //break;
            //}
            if (choice == 1) {
                double new_income;
                printf("Enter new monthly income: ");
                scanf("%lf", &new_income);
                if (update_user_field(current_user, "income", new_income))
                    printf("Income updated!\n");
                else
                    printf("Failed to update income.\n");
            } else if (choice == 2) {
                double new_expense;
                printf("Enter new monthly expense: ");
                scanf("%lf", &new_expense);
                if (update_user_field(current_user, "expense", new_expense))
                    printf("Expense updated!\n");
                else
                    printf("Failed to update expense.\n");
            } else if (choice == 3) {
                char type[16], category[32], notes[128], month[16];
                double amount;

                while (1) {
                    printf("Enter type (income/expense): ");
                    scanf("%15s", type);

                    if (strcmp(type, "income") == 0 || strcmp(type, "expense") == 0) {
                        break; // valid input
                    } else {
                        printf("Invalid type! Please type 'income' or 'expense'.\n");
                    }
                }

                printf("Enter month (YYYY-MM / 2025-01): ");
                scanf("%15s", &month);

                printf("Enter amount: ");
                scanf("%lf", &amount);
                getchar(); // clear newline
                printf("Enter category: ");
                scanf("%31s", category);
                getchar();
                printf("Enter notes: ");
                fgets(notes, sizeof(notes), stdin);
                notes[strcspn(notes, "\n")] = 0; // remove newline

                if (add_transaction(current_user, type, month, amount, category, notes))
                    printf("Transaction added!\n");
                else
                    printf("Failed to save transaction.\n");
            } else if (choice == 4) {
                char month[16];
                printf("Enter month (YYYY-MM / 2025-01): ");
                scanf("%15s", month);
                view_transactions_by_month(current_user, month);
            } else {
                printf("Invalid option.\n");
            }
        }
    }
    return 0;
}
