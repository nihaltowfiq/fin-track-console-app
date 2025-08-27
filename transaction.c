#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "./transaction.h"
#include "./config.h"

void get_datetime(char *month, size_t month_size, char *date, size_t date_size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Fill month and date
    strftime(month, month_size, "%Y-%m", t);    // Year-Month
    strftime(date, date_size, "%Y-%m-%d", t);   // Full date
}

bool add_transaction(const char *username, const char *type, double amount, const char *category, const char *notes) {
    char filename[128];
    snprintf(filename, sizeof(filename), "data/%s_transactions.csv", username);

    FILE *f = fopen(filename, "a");
    if (!f) {
        perror("File open failed");
        return false;
    }

    char month[16], date[16];
    get_datetime(month, sizeof(month), date, sizeof(date));

    // Save transaction to CSV
    fprintf(f, "%s,%.2f,%s,%s,%s,%s\n", type, amount, category, month, date, notes);
    fclose(f);
    return true;
}
