#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./config.h"

// Update or insert monthly budget for a user
bool update_budget(const char *username, const char *month, double amount) {
    char filename[128];
    snprintf(filename, sizeof(filename), "data/%s_budgets.csv", username);

    // Load existing budgets into memory
    FILE *f = fopen(filename, "r");
    char lines[1000][64];
    int count = 0;
    int updated = 0;

    if (f) {
        while (fgets(lines[count], sizeof(lines[count]), f)) {
            char existing_month[16];
            double existing_amount;

            if (sscanf(lines[count], "%15[^,],%lf", existing_month, &existing_amount) == 2) {
                if (strcmp(existing_month, month) == 0) {
                    snprintf(lines[count], sizeof(lines[count]), "%s,%.2f\n", month, amount);
                    updated = 1;
                }
            }
            count++;
        }
        fclose(f);
    }

    // If month not found, add it
    if (!updated) {
        snprintf(lines[count], sizeof(lines[count]), "%s,%.2f\n", month, amount);
        count++;
    }

    // Write all back
    f = fopen(filename, "w");
    if (!f) return false;
    for (int i = 0; i < count; i++) {
        fputs(lines[i], f);
    }
    fclose(f);
    return true;
}

// Get budget for a month
double get_budget(const char *username, const char *month) {
    char filename[128];
    snprintf(filename, sizeof(filename), "data/%s_budgets.csv", username);

    FILE *f = fopen(filename, "r");
    if (!f) return 0.0;

    char line[64], file_month[16];
    double amount;
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%15[^,],%lf", file_month, &amount) == 2) {
            if (strcmp(file_month, month) == 0) {
                fclose(f);
                return amount;
            }
        }
    }
    fclose(f);
    return 0.0;
}
