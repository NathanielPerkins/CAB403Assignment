#include "fileControl.h"


int8_t get_file_dimensions(const char *file_location, int16_t *lines, int16_t *width){
    FILE *fp;
    int8_t current_char;
    int16_t max_width = 0;
    int16_t curr_width = 0;
    fp = fopen(file_location,"r");
    if (fp == NULL) {
        printf("gfd: Couldn't open file: %s\n",file_location);
        return 0;
    }
    do {
        curr_width++;
        current_char = fgetc(fp);
        if (current_char == '\n'){
            (*lines)++;
            if (curr_width > max_width) max_width = curr_width;
            curr_width = 0;
        }
    } while(!feof(fp));
    (*width) = max_width;
    fclose(fp);
    return 1;
}


int8_t open_Accounts(struct Account **all_accounts){
    const char* accounts = "Accounts.txt";
    int16_t num_accounts;
    int16_t width;
    int8_t success = get_file_dimensions(accounts,&num_accounts,&width);
    printf("Opening Accounts.txt\n");
    if(!success){
        printf("open_Accounts function closing with couldn't open file error\n");
        return 0;
    }
    *all_accounts = (struct Account*)malloc(sizeof(struct Account)*(num_accounts));
    FILE *fp;
    fp = fopen(accounts,"r");
    if (fp == NULL) {
        printf("oa: Couldn't Open file: %s\n",accounts);
    }
    while(fgetc(fp) != '\n') {} // parse over the header
    int8_t current_char = ' ';
    int8_t num = 0;
    do {
        // Look into strtok and strdup for file I/O
        char line[width];
        int8_t i = 0;
        while((current_char = fgetc(fp)) != '\n'){
            line[i] = current_char;
            i++;
        }
        char *token;
        token = strtok(line, " "); //tokenize the string, removing " " characters
        (*all_accounts)[num].accountNo = strtol(token,NULL,10);
        token = strtok(NULL, " "); // advance to next filled section
        (*all_accounts)[num].openBal = strtod(token,NULL);
        token = strtok(NULL, " "); // advance to next filled section
        (*all_accounts)[num].closeBal = strtod(token,NULL);

        num++;
        if (num == num_accounts - 1){
            while(!feof(fp)) fgetc(fp); //Advance to the end of file
        }
    } while (!feof(fp));
    fclose(fp);
    printf("Closing Accounts.txt\n");
    return num_accounts - 1;
}

int8_t open_Client_details(struct Account **all_accounts){

    return 1;
}
