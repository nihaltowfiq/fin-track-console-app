#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./auth.h"
#include "./config.h"

bool signup(const char *username, const char *password) {
    FILE *f = fopen(USERS_DB_PATH, "r");
    if (f) {
        char u[USERNAME_MAX], p[PASSWORD_MAX];
        while (fscanf(f, "%31[^,],%63[^\n]\n", u, p) == 2) {
            if (strcmp(u, username) == 0) {
                fclose(f);
                return false;
            }
        }
        fclose(f);
    }

    f = fopen(USERS_DB_PATH, "a");
    if (!f) return false;
    fprintf(f, "%s,%s\n", username, password);
    fclose(f);
    return true;
}

bool signin(const char *username, const char *password) {
    FILE *f = fopen(USERS_DB_PATH, "r");
    if (!f) return false;

    char u[USERNAME_MAX], p[PASSWORD_MAX];
    while (fscanf(f, "%31[^,],%63[^\n]\n", u, p) == 2) {
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
            fclose(f);
            return true;
        }
    }

    fclose(f);
    return false;
}

