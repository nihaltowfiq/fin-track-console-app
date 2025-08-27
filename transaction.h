#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdbool.h>

bool add_transaction(const char *username, const char *type, double amount, const char *category, const char *notes);

#endif
