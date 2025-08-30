#ifndef GOALS_H
#define GOALS_H

#include <stdbool.h>

bool set_savings_goal(const char *username, const char *goal_name, double target_amount, const char *target_month);
void show_savings_progress(const char *username, const char *month);

#endif

