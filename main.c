#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./auth.h"
#include "./config.h"
#include "./transaction.h"
#include "./budget.h"

int main() {
    int choice;
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    int is_authenticated = 0; // track login state
    char current_user[USERNAME_MAX];

    while (1) {
        printf("\n==== FinTrack ====\n");
        if (!is_authenticated) {
            printf("1. Sign In\n");
            printf("2. Sign Up\n");
            printf("3. Exit\n");
        } else {
            printf("1. Update Budget\n");
            printf("2. Add Transaction\n");
            printf("3. View Transactions\n");
            printf("4. Get Statements\n");
            printf("5. Sign Out\n");
            printf("6. Exit\n");
        }
        printf("Choose: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        if (!is_authenticated) {
            if (choice == 3) {
                printf("Goodbye!\n");
                break;
            }

            printf("Username: ");
            scanf("%31s", username);
            printf("Password: ");
            scanf("%63s", password);

            if (choice == 1) {
                if (signin(username, password)) {
                    printf("Login successful! Welcome %s\n", username);
                    is_authenticated = 1;
                    strcpy(current_user, username);
                } else {
                    printf("Login failed! Incorrect credentials.\n");
                }

            } else if (choice == 2) {
                if (signup(username, password))
                    printf("Signup successful!\nPlease Login!\n");
                else
                    printf("Signup failed! Username may already exist.\n");
            } else {
                printf("Invalid option.\n");
            }
        } else {
            // If sign in
            if (choice == 1) {
                char month[16];
                double amount;
                printf("Enter month (YYYY-MM / 2025-01): ");
                scanf("%15s", month);
                printf("Enter budget amount: ");
                scanf("%lf", &amount);

                if (update_budget(current_user, month, amount))
                    printf("Budget updated for %s: %.2f\n", month, amount);
                else
                    printf("Failed to update budget.\n");
            } else if (choice == 2) {
                char type[16], category[32], notes[128], month[16];
                double amount;

                // Ask for transaction type
                while (1) {
                    printf("Enter type (income/expense): ");
                    scanf("%15s", type);
                    if (strcmp(type, "income") == 0 || strcmp(type, "expense") == 0) {
                        break;
                    } else {
                        printf("Invalid type! Please type 'income' or 'expense'.\n");
                    }
                }

                // Ask for month
                printf("Enter month (YYYY-MM / 2025-01): ");
                scanf("%15s", month);

                //Check if this month exists in budgets.csv
                double current_budget = get_budget(current_user, month);
                if (current_budget == 0.0) {
                    printf("No budget found for %s. Enter budget amount: ", month);
                    double new_budget;
                    scanf("%lf", &new_budget);
                    if (update_budget(current_user, month, new_budget)) {
                        printf("Budget added for %s: %.2f\n", month, new_budget);
                    } else {
                        printf("Failed to add budget.\n");
                    }
                }

                // Ask for transaction details
                printf("Enter amount: ");
                scanf("%lf", &amount);
                getchar(); // clear newline

                printf("Enter category: ");
                scanf("%31s", category);
                getchar();

                printf("Enter notes: ");
                fgets(notes, sizeof(notes), stdin);
                notes[strcspn(notes, "\n")] = 0; // remove newline

                // Save transaction
                if (add_transaction(current_user, type, month, amount, category, notes))
                    printf("Transaction added!\n");
                else
                    printf("Failed to save transaction.\n");
            } else if (choice == 3) {
                char month[16];
                printf("Enter month (YYYY-MM / 2025-01): ");
                scanf("%15s", month);
                view_transactions_by_month(current_user, month);
            } else if(choice == 4) {
                char month[16];
                printf("Enter month (YYYY-MM / 2025-01): ");
                scanf("%15s", month);
                get_statements_by_month(current_user, month);
            } else if (choice == 5) {
                printf("Signed out.\n");
                is_authenticated = 0;
            } else if (choice == 6) {
                printf("Goodbye!\n");
                break;
            } else {
                printf("Invalid option.\n");
            }
        }
    }
    return 0;
}
