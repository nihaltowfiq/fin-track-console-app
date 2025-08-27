#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

bool signup(const char *username, const char *password, double income, double expense);
bool login(const char *username, const char *password);
bool update_user_field(const char *username, const char *field, double value);

#endif
