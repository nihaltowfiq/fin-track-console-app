#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "./menu.h"
#include "./auth.h"
#include "./budget.h"
#include "./transaction.h"
#include "./goal.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int is_valid_month(const char *month) {
    if (strlen(month) != 7) return 0;
    return isdigit(month[0]) && isdigit(month[1]) &&
           isdigit(month[2]) && isdigit(month[3]) &&
           month[4] == '-' &&
           isdigit(month[5]) && isdigit(month[6]);
}

void read_string(const char *prompt, char *buffer, size_t size) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin)) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 0) return;
        }
        printf("Invalid input. Please try again.\n");
    }
}

double read_positive_double(const char *prompt) {
    double value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &value) == 1 && value >= 0) {
            clear_input_buffer();
            return value;
        }
        printf("Invalid number. Must be a non-negative value.\n");
        clear_input_buffer();
    }
}

void show_guest_menu() {
    printf("1. Sign In\n");
    printf("2. Sign Up\n");
    printf("3. Exit\n");
}

void show_user_menu() {
    printf("\n==============================\n");
    printf("     FinTrack Dashboard    \n");
    printf("==============================\n");
    printf("1. Update Expense Budget\n");
    printf("2. Add Transaction\n");
    printf("3. View Transactions\n");
    printf("4. Get Statements\n");
    printf("5. Delete Transaction\n");
    printf("6. Set Savings Goal\n");
    printf("7. View Savings Progress\n");
    printf("8. Sign Out\n");
    printf("9. Exit\n");
}

void handle_guest_choice(int choice, int *is_authenticated, char *current_user) {
    char username[USERNAME_MAX], password[PASSWORD_MAX];

    switch (choice) {
        case 1:
            read_string("Username: ", username, sizeof(username));
            read_string("Password: ", password, sizeof(password));

            if (signin(username, password)) {
                printf("Signin successful! Welcome %s\n", username);
                *is_authenticated = 1;
                strcpy(current_user, username);
            } else {
                printf("Signin failed! Incorrect credentials.\n");
            }
            break;

        case 2:
            read_string("Username: ", username, sizeof(username));
            read_string("Password: ", password, sizeof(password));

            if (signup(username, password))
                printf("Signup successful! Please login.\n");
            else
                printf("Signup failed! Username may already exist.\n");
            break;

        case 3:
            printf("Goodbye!\n");
            break;

        default:
            printf("Invalid option. Try again.\n");
    }
}

void handle_user_choice(int choice, int *is_authenticated, const char *current_user) {
    char month[16], type[16], category[32], notes[128];
    double amount;

    switch (choice) {
        case 1:
            while (1) {
                read_string("Enter month (YYYY-MM): ", month, sizeof(month));
                if (is_valid_month(month)) break;
                printf("Invalid month format! Example: 2025-01\n");
            }
            amount = read_positive_double("Enter budget amount: ");
            if (update_budget(current_user, month, amount))
                printf("Budget updated for %s: %.2f\n", month, amount);
            else
                printf("Failed to update budget.\n");
            break;

        case 2:
            while (1) {
                read_string("Enter type (income/expense): ", type, sizeof(type));
                if (strcmp(type, "income") == 0 || strcmp(type, "expense") == 0) break;
                printf("Invalid type! Please type 'income' or 'expense'.\n");
            }
            while (1) {
                read_string("Enter month (YYYY-MM): ", month, sizeof(month));
                if (is_valid_month(month)) break;
                printf("Invalid month format! Example: 2025-01\n");
            }
            {
                double current_budget = get_budget(current_user, month);
                if (current_budget == 0.0) {
                    printf("No expense budget found for %s.\n", month);
                    double new_budget = read_positive_double("Enter expense budget for this month: ");
                    if (update_budget(current_user, month, new_budget))
                        printf("Budget added for %s: %.2f\n", month, new_budget);
                }
            }
            amount = read_positive_double("Enter amount: ");
            read_string("Enter category: ", category, sizeof(category));
            read_string("Enter notes: ", notes, sizeof(notes));

            if (add_transaction(current_user, type, month, amount, category, notes))
                printf("Transaction added!\n");
            else
                printf("Failed to save transaction.\n");
            break;

        case 3:
            while (1) {
                read_string("Enter month (YYYY-MM): ", month, sizeof(month));
                if (is_valid_month(month)) break;
                printf("Invalid month format! Example: 2025-01\n");
            }
            view_transactions_by_month(current_user, month);
            break;

        case 4:
            while (1) {
                read_string("Enter month (YYYY-MM): ", month, sizeof(month));
                if (is_valid_month(month)) break;
                printf("Invalid month format! Example: 2025-01\n");
            }
            get_statements_by_month(current_user, month);
            break;

        case 5: {
            int txn_id;
            printf("Enter Transaction ID to delete: ");
            if (scanf("%d", &txn_id) != 1) {
                printf("Invalid input. Must be a number.\n");
                clear_input_buffer();
                break;
            }
            clear_input_buffer();
            if (delete_transaction(current_user, txn_id))
                printf("Transaction %d deleted successfully.\n", txn_id);
            else
                printf("Transaction not found.\n");
            break;
        }

        case 6: {
            char goal_name[64], goal_month[16];
            double goal_amount;
            read_string("Enter goal name: ", goal_name, sizeof(goal_name));
            goal_amount = read_positive_double("Enter target amount: ");
            while (1) {
                read_string("Enter target month (YYYY-MM): ", goal_month, sizeof(goal_month));
                if (is_valid_month(goal_month)) break;
                printf("Invalid month format! Example: 2025-12\n");
            }
            if (set_savings_goal(current_user, goal_name, goal_amount, goal_month))
                printf("Savings goal added!\n");
            else
                printf("Failed to add goal.\n");
            break;
        }

        case 7:
            while (1) {
                read_string("Enter month (YYYY-MM): ", month, sizeof(month));
                if (is_valid_month(month)) break;
                printf("Invalid month format! Example: 2025-01\n");
            }
            show_savings_progress(current_user, month);
            break;

        case 8:
            printf("Signed out.\n");
            *is_authenticated = 0;
            break;

        case 9:
            printf("Goodbye!\n");
            exit(0);

        default:
            printf("Invalid option. Try again.\n");
    }
}
