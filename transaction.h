#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdbool.h>

typedef struct {
    int id;
    char type[16];
    char month[16];
    double amount;
    char category[32];
    char date[16];
    char notes[128];
} Transaction;

bool add_transaction(const char *username, const char *type, const char *month, double amount, const char *category, const char *notes);
bool delete_transaction(const char *username, int id);
void view_transactions_by_month(const char *username, const char *month);
void get_statements_by_month(const char *username, const char *month);
double get_budget(const char *username, const char *month);


#endif
