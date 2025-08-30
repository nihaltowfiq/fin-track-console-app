#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./transaction.h"
#include "./goal.h"
#include "./config.h"

bool set_savings_goal(const char *username, const char *goal_name, double target_amount, const char *target_month) {
    char filename[128];
    snprintf(filename, sizeof(filename), GOAL_DB_PATH, username);

    FILE *f = fopen(filename, "a");
    if (!f) return false;

    fprintf(f, "%s,%.2f,%s\n", goal_name, target_amount, target_month);
    fclose(f);
    return true;
}

void show_savings_progress(const char *username, const char *month) {
    char goals_file[128], txn_file[128];
    snprintf(goals_file, sizeof(goals_file), GOAL_DB_PATH, username);
    snprintf(txn_file, sizeof(txn_file), TRANSACTION_DB_PATH, username);

    FILE *g = fopen(goals_file, "r");
    if (!g) {
        printf("No savings goals found.\n");
        return;
    }

    FILE *t = fopen(txn_file, "r");
    if (!t) {
        printf("No transactions found.\n");
        fclose(g);
        return;
    }

    double total_income = 0, total_expense = 0;
    Transaction tx;
    int id;

    while (fscanf(t, "%d,%15[^,],%15[^,],%lf,%31[^,],%15[^,],%127[^\n]\n",
                  &id, tx.type, tx.month, &tx.amount, tx.category, tx.date, tx.notes) == 7) {
        if (strcmp(tx.month, month) == 0) {
            if (strcmp(tx.type, "income") == 0) total_income += tx.amount;
            else if (strcmp(tx.type, "expense") == 0) total_expense += tx.amount;
        }
    }
    fclose(t);

    double net_savings = total_income - total_expense;

    printf("\n=================== SAVINGS GOALS (%s) ====================\n", month);
    char goal_name[64], target_month[16];
    double target_amount;
    while (fscanf(g, "%63[^,],%lf,%15[^\n]\n", goal_name, &target_amount, target_month) == 3) {
        double progress = (net_savings / target_amount) * 100;
        if (progress > 100) progress = 100;
        printf("Goal: %-20s | Target: %.2f | Deadline: %s | Progress: %.2f%%\n",
               goal_name, target_amount, target_month, progress);
    }
    fclose(g);
}
