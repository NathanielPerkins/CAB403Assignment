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
    const char* accounts = "Database/Accounts.txt";
    int16_t num_accounts;
    int16_t width;
    int8_t success = get_file_dimensions(accounts,&num_accounts,&width);
    if(!success){
        printf("open_Accounts function closing with couldn't open file error\n");
        return 0;
    }
    *all_accounts = (struct Account*)malloc(sizeof(struct Account)*(num_accounts-1));
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
        token = strtok(line, " ");
        while(token != NULL){
            printf("token = %s\n",token);
            printf("Accessing all_accounts[%d]\n",num);
            all_accounts[num]->accountNo = atoi(token);
            token = strtok(NULL, " ");
            printf("Finished\n");
        }
        printf("%d\n",num);
        printf("Account[%d].accountNo = %d",num,all_accounts[num]->accountNo);
        num++;
    } while (!feof(fp));
    printf("EXITED");
    fclose(fp);
    return 1;
}
