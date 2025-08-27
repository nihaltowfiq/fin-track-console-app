#include <direct.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "./auth.h"
#include "./config.h"

// Ensure data folder exists
void ensure_data_folder() {
#if defined(_WIN32)
    _mkdir("data");
#else
    mkdir("data", 0775);
#endif
}

// Sign up a new user (CSV: username,password, income, budget)
bool signup(const char *username, const char *password, double income, double budget) {
    ensure_data_folder();

    // Check if user already exists
    FILE *f = fopen(USERS_DB_PATH, "r");
    if (f) {
        char u[USERNAME_MAX], p[PASSWORD_MAX];
        double in, ex;
        while (fscanf(f, "%31[^,],%63[^,],%lf,%lf\n", u, p, &in, &ex) == 4) {
            if (strcmp(u, username) == 0) {
                fclose(f);
                return false; // user exists
            }
        }
        fclose(f);
    }

    // Append new user with CSV fields
    f = fopen(USERS_DB_PATH, "a");
    if (!f) return false;
    fprintf(f, "%s,%s,%.2f,%.2f\n", username, password, income, budget);
    fclose(f);
    return true;
}

// Login user (checks CSV)
bool login(const char *username, const char *password) {
    ensure_data_folder();

    FILE *f = fopen(USERS_DB_PATH, "r");
    if (!f) return false;

    char u[USERNAME_MAX], p[PASSWORD_MAX];
    double in, ex;
    while (fscanf(f, "%31[^,],%63[^,],%lf,%lf\n", u, p, &in, &ex) == 4) {
        if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
            fclose(f);
            return true; // login success
        }
    }

    fclose(f);
    return false; // login failed
}

// Update user income or expense
bool update_user_field(const char *username, const char *field, double value) {
    FILE *f = fopen(USERS_DB_PATH, "r");
    if (!f) return false;

    FILE *temp = fopen("data/temp.csv", "w");
    if (!temp) {
        fclose(f);
        return false;
    }

    char u[USERNAME_MAX], p[PASSWORD_MAX];
    double in, ex;
    int updated = 0;

    while (fscanf(f, "%31[^,],%63[^,],%lf,%lf\n", u, p, &in, &ex) == 4) {
        if (strcmp(u, username) == 0) {
            if (strcmp(field, "income") == 0)
                in = value;
            else if (strcmp(field, "expense") == 0)
                ex = value;
            updated = 1;
        }
        fprintf(temp, "%s,%s,%.2f,%.2f\n", u, p, in, ex);
    }

    fclose(f);
    fclose(temp);

    // Replace old file
    remove(USERS_DB_PATH);
    rename("data/temp.csv", USERS_DB_PATH);

    return updated;
}
