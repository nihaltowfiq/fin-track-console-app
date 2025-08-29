#ifndef MENU_H
#define MENU_H

#include "./config.h"

// Show menus
void show_guest_menu();
void show_user_menu();

// Handle choices
void handle_guest_choice(int choice, int *is_authenticated, char *current_user);
void handle_user_choice(int choice, int *is_authenticated, const char *current_user);

#endif

