#ifndef _FILECONTROL_H
#define _FILECONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Account {
    int16_t accountNo;
    int16_t opening_balance;
    int16_t closing_balance;
};

int8_t get_file_dimensions(const char *file_location, int16_t *lines, int16_t *width);
int8_t open_Accounts(struct Account **all_accounts);

#endif
