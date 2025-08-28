#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdbool.h>

bool add_transaction(const char *username, const char *type, const char *month, double amount, const char *category, const char *notes);
void view_transactions_by_month(const char *username, const char *month);
void get_statements_by_month(const char *username, const char *month);


#endif
