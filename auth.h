#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

bool signup(const char *username, const char *password);
bool signin(const char *username, const char *password);
bool update_user_field(const char *username, const char *field, double value);

#endif
