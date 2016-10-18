#ifndef _FILECONTROL_H
#define _FILECONTROL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Account {
    int32_t accountNo;
    double openBal;
    double currentBal;
    double closeBal;
    int8_t type;
};

struct Client_Details{
    char firstN[20];
    char lastN[20];
    char username[20];
    int32_t pin;
    int32_t clientNo;
    struct Account *loan;       // can only have 1
    struct Account *cheque;     // can only have 1
    struct Account *savings;    // can only have 1
};

struct Transaction{
    struct Account *fromAcct;
    struct Account *toAcct;
    double amount;
    int8_t type;
};

int8_t get_file_dimensions(const char *file_location, int16_t *lines, int16_t *width);
/*
open_accounts() opens the Accounts.txt file, located in the same directory
parsing over the file, tokenizing and then creating an array of Account structs
to pass back to the prior function
inputs: struct Account **all_accounts
memory address to desired array of structs
return: int8_t
number of accounts in the array
*/
int8_t open_Accounts(struct Account **all_accounts);

int8_t open_Client_details(struct Account **all_accounts);

#endif
