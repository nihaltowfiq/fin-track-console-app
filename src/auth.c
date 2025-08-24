#include "../include/auth.h"
#include "../include/config.h"
#include <direct.h> // for _mkdir on Windows
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

// Ensure data folder exists
void ensure_data_folder() {
#if defined(_WIN32)
  _mkdir("data");
#else
  mkdir("data", 0775);
#endif
}

// Sign up a new user (plain text storage)
bool signup(const char *username, const char *password) {
  ensure_data_folder();

  // Check if user already exists
  FILE *f = fopen(USERS_DB_PATH, "r");
  if (f) {
    char u[USERNAME_MAX], p[PASSWORD_MAX];
    while (fscanf(f, "%31s %63s", u, p) == 2) {
      if (strcmp(u, username) == 0) {
        fclose(f);
        return false; // user exists
      }
    }
    fclose(f);
  }

  // Append new user
  f = fopen(USERS_DB_PATH, "a");
  if (!f) return false;
  fprintf(f, "%s %s\n", username, password);
  fclose(f);
  return true;
}

// Login user (plain text check)
bool login(const char *username, const char *password) {
  ensure_data_folder();

  FILE *f = fopen(USERS_DB_PATH, "r");
  if (!f) return false;

  char u[USERNAME_MAX], p[PASSWORD_MAX];
  while (fscanf(f, "%31s %63s", u, p) == 2) {
    if (strcmp(u, username) == 0 && strcmp(p, password) == 0) {
      fclose(f);
      return true; // login success
    }
  }

  fclose(f);
  return false; // login failed
}
