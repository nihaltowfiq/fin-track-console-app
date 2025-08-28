#ifndef BUDGET_H
#define BUDGET_H

#include <stdbool.h>

bool update_budget(const char *username, const char *month, double amount);
double get_budget(const char *username, const char *month);

#endif

