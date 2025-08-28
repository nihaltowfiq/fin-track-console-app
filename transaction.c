#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "./transaction.h"
#include "./config.h"
#include "./budget.h"

void get_datetime(char *date, size_t date_size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Fill month and date
    // strftime(month, month_size, "%Y-%m", t);    // Year-Month
    strftime(date, date_size, "%Y-%m-%d", t);   // Full date
}

bool add_transaction(const char *username, const char *type, const char *month, double amount, const char *category, const char *notes) {
    char filename[128];
    snprintf(filename, sizeof(filename), "data/%s_transactions.csv", username);

    FILE *f = fopen(filename, "a");
    if (!f) {
        perror("File open failed");
        return false;
    }

    char date[16];
    get_datetime(date, sizeof(date));


    fprintf(f, "%s,%s,%.2f,%s,%s,%s\n", type,  month, amount, category, date, notes);
    fclose(f);
    return true;
}

// Struct for a transaction
typedef struct {
    char type[16];
    double amount;
    char category[32];
    char month[16];
    char date[16];
    char notes[128];
} Transaction;


// View transactions for a given month
void view_transactions_by_month(const char *username, const char *target_month) {
    char filename[128];
    snprintf(filename, sizeof(filename), "data/%s_transactions.csv", username);

    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("No transactions found for user %s.\n", username);
        return;
    }

    Transaction txns[1000];
    int count = 0;

    // Updated fscanf to match new CSV order
    while (fscanf(f, "%15[^,],%15[^,],%lf,%31[^,],%15[^,],%127[^\n]\n",
                  txns[count].type,
                  txns[count].month,
                  &txns[count].amount,
                  txns[count].category,
                  txns[count].date,
                  txns[count].notes) == 6) {
        if (strcmp(txns[count].month, target_month) == 0) {
            count++;
        }
    }
    fclose(f);

    if (count == 0) {
        printf("No transactions found for %s in %s.\n", username, target_month);
        return;
    }

    double total_income = 0.0, total_expense = 0.0;

    printf("\n==== Transactions for %s ====\n", target_month);

    // Show INCOME
    printf("\n--- Income ---\n");
    printf("%-15s %-10s %s\n", "Category", "Amount", "Notes");
    for (int i = 0; i < count; i++) {
        if (strcmp(txns[i].type, "income") == 0) {
            printf("%-15s %-10.2f %s\n", txns[i].category, txns[i].amount, txns[i].notes);
            total_income += txns[i].amount;
        }
    }
    printf("Total Income: %.2f\n", total_income);

    // Show EXPENSE
    printf("\n--- Expense ---\n");
    printf("%-15s %-10s %s\n", "Category", "Amount", "Notes");
    for (int i = 0; i < count; i++) {
        if (strcmp(txns[i].type, "expense") == 0) {
            printf("%-15s %-10.2f %s\n", txns[i].category, txns[i].amount, txns[i].notes);
            total_expense += txns[i].amount;
        }
    }
    printf("Total Expense: %.2f\n", total_expense);

    // Budget check
    double budget = get_budget(username, target_month);

    printf("\nBudget: %.2f\n", budget);
    if (total_expense > budget) {
        printf("WARNING: You have exceeded your budget!\n");
    } else {
        printf("You are within your budget.\n");
    }
}
