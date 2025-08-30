#ifndef MENU_H
#define MENU_H

#include "./config.h"

void show_guest_menu();
void show_user_menu();
void handle_guest_choice(int choice, int *is_authenticated, char *current_user);
void handle_user_choice(int choice, int *is_authenticated, const char *current_user);

#endif

