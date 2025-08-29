#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "./transaction.h"
#include "./config.h"
#include "./budget.h"

typedef struct {
    char type[16];
    double amount;
    char category[32];
    char month[16];
    char date[16];
    char notes[128];
} Transaction;

void get_datetime(char *date, size_t date_size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(date, date_size, "%Y-%m-%d", t);
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

void view_transactions_by_month(const char *username, const char *target_month) {
    char filename[128];
    snprintf(filename, sizeof(filename), "data/%s_transactions.csv", username);

    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("\nNo transactions found for user %s.\n", username);
        return;
    }

    Transaction txns[1000];
    int count = 0;

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
        printf("\nNo transactions found for %s in %s.\n", username, target_month);
        return;
    }

    double total_income = 0.0, total_expense = 0.0;

    printf("\n============================================================================\n");
    printf("                           Transactions for %s\n", target_month);
    printf("============================================================================\n");

    printf("\n================================= INCOME ===================================\n");
    printf("%-15s | %-10s | %-12s | %-30s\n", "Category", "Amount", "Created At", "Notes");
    printf("----------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(txns[i].type, "income") == 0) {
            printf("%-15s | %-10.2f | %-12s | %-30s\n",
                   txns[i].category, txns[i].amount, txns[i].date, txns[i].notes);
            total_income += txns[i].amount;
        }
    }
    printf("----------------------------------------------------------------------------\n");
    printf("TOTAL INCOME: %.2f\n", total_income);

    printf("\n================================ EXPENSE ===================================\n");
    printf("%-15s | %-10s | %-12s | %-30s\n", "Category", "Amount", "Created At", "Notes");
    printf("----------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(txns[i].type, "expense") == 0) {
            printf("%-15s | %-10.2f | %-12s | %-30s\n",
                   txns[i].category, txns[i].amount, txns[i].date, txns[i].notes);
            total_expense += txns[i].amount;
        }
    }
    printf("----------------------------------------------------------------------------\n");
    printf("TOTAL EXPENSE: %.2f\n", total_expense);

    double budget = get_budget(username, target_month);
    printf("\n============================== BUDGET CHECK ================================\n");
    printf("Budget for %s: %.2f\n", target_month, budget);
    if (total_expense > budget) {
        printf("WARNING: You have exceeded your budget!\n");
    } else {
        printf("You are within your budget.\n");
    }
    printf("============================================================================\n");
}


void get_statements_by_month(const char *username, const char *target_month) {
    char filename[128];
    snprintf(filename, sizeof(filename), "data/%s_transactions.csv", username);

    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("\nNo transactions found for user %s.\n", username);
        return;
    }

    Transaction txns[1000];
    int count = 0;

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
        printf("\nNo transactions found for %s in %s.\n", username, target_month);
        return;
    }

    double total_income = 0.0, total_expense = 0.0;

    printf("\n============================================================================\n");
    printf("                        Monthly Statement for %s\n", target_month);
    printf("============================================================================\n");

    printf("%-10s | %-15s | %-10s | %-12s | %-30s\n", "Type", "Category", "Amount", "Created At", "Notes");
    printf("----------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-10s | %-15s | %-10.2f | %-12s | %-30s\n",
               txns[i].type, txns[i].category, txns[i].amount, txns[i].date, txns[i].notes);

        if (strcmp(txns[i].type, "income") == 0) {
            total_income += txns[i].amount;
        } else if (strcmp(txns[i].type, "expense") == 0) {
            total_expense += txns[i].amount;
        }
    }

    printf("----------------------------------------------------------------------------\n");

    double savings = total_income - total_expense;

    printf("\n=========================== SUMMARY ===========================\n");
    printf("Total Income : %.2f\n", total_income);
    printf("Total Expense: %.2f\n", total_expense);
    printf("Savings      : %.2f\n", savings);
    printf("================================================================\n");

    double budget = get_budget(username, target_month);
    printf("Budget for %s: %.2f\n", target_month, budget);
    if (total_expense > budget) {
        printf("Status: Over Budget!\n");
    } else {
        printf("Status: Within Budget.\n");
    }
    printf("================================================================\n");
}


