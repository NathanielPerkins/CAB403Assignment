#include "fileControl.h"


int8_t get_file_dimensions(const char *file_location, int16_t *lines, int16_t *width){
    FILE *fp;
    int8_t current_char;
    int16_t max_width = 0;
    int16_t curr_width = 0;
    (*lines) = 0;
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

int8_t open_Clients(struct Client **clients){
    const char* file_name = "Client_Details.txt";
    int16_t num_clients;
    int16_t width;
    int8_t success = get_file_dimensions(file_name,&num_clients,&width);
    FILE *fp;
    if(!success){
        printf("OC: function closing with couldn't open file error\n");
        return 0;
    }
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("OC: Couldn't Open file: %s\n",file_name);
    }
    *clients = (struct Client*)malloc(sizeof(struct Client)*(num_clients));
    char current_char = ' ';
    int8_t num = 0;
    while(fgetc(fp) != '\n') {} // parse over the header
    do {
        Client_Init(&(*clients)[num]);
        int8_t i = 0;
        char line[width];
        memset(line,0,width);
        while((current_char = fgetc(fp)) != '\n'){
            line[i] = current_char;
            i++;
        }

        char *token;

        token = strtok(line, " "); //tokenize the string up till first " ": First Name
        // printf("Line[%d]:\t",num);
        memcpy((*clients)[num].firstN,token,strlen(token)+1);
        // printf("|%s|",(*clients)[num].firstN);

        token = strtok(NULL, " "); // advance to next filled section: Last Name
        memcpy((*clients)[num].lastN,token,strlen(token)+1);
        // printf("|%s|",(*clients)[num].lastN);

        token = strtok(NULL, " "); // advance to next filled section: ClientNo
        (*clients)[num].clientNo = strtol(token,NULL,10);
        // printf("CN: %d\t",(*clients)[num].clientNo);
        token = strtok(NULL, "\n");

        int8_t num_accounts = 1;
        for (int j = 0; token[j] != '\0';j++){
            if (token[j] == ',') num_accounts++;
        }
        char *token2;
        token2 = strtok(token, " ");
        // token2 = strtok(NULL, ",");
        // printf("%s",token2);
        char account[9];
        for (int j = 0; j<num_accounts; j++){
            for (int k = 0; k<8; k++){
                account[k] = token2[k];
            }
            account[8] = '\0';
            int32_t acctNo = strtol(account,NULL,10);
            if      (acctNo%11 == 0) (*clients)[num].savAcctNo = acctNo;
            else if (acctNo%12 == 0) (*clients)[num].loanAcctNo = acctNo;
            else if (acctNo%13 == 0) (*clients)[num].chequeAcctNo = acctNo;
            else printf("Account number %d shouldn't exist",acctNo);
            token2 += 9; // Account number is 8 long + ','
        }
        num++;
        if (num == num_clients - 1){
            while(!feof(fp)) fgetc(fp); //Advance to the end of file
        }
    } while (!feof(fp));


    fclose(fp);
    return num_clients - 1;
}

int8_t open_auth(struct Client *clients){
    printf("Entered\n");
    const char* file_name = "Authentication.txt";
    int16_t num_clients;
    int16_t width;
    int8_t success = get_file_dimensions(file_name,&num_clients,&width);
    printf("Test\n");
    FILE *fp;
    if(!success){
        printf("OA: function closing with couldn't open file error\n");
        return 0;
    }
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("OA: Couldn't Open file: %s\n",file_name);
    }
    printf("Opening %s\n",file_name);
    char current_char = ' ';
    int8_t num = 0;
    while(fgetc(fp) != '\n') {} // parse over the header
    do {
        // printf("Line[%d]\n",num);
        int8_t i = 0;
        char line[width];
        memset(line,0,width);
        while((current_char = fgetc(fp)) != '\n'){
            line[i] = current_char;
            i++;
        }
        char *token;
        token = strtok(line, " \n\t"); //tokenize the string, removing " " characters
        memcpy(clients[num].username,token,strlen(token));
        token = strtok(NULL, " \n\t"); // advance to next filled section
        clients[num].pin = strtol(token,NULL,10);
        token = strtok(NULL, " \n\t"); // advance to next filled section
        clients[num].clientNo = strtol(token,NULL,10);
        num++;
        if (num == num_clients - 1){
            printf("clients: %d\n",num_clients);
            while(!feof(fp)) fgetc(fp); //Advance to the end of file
        }
    } while (!feof(fp));


    fclose(fp);
    return num_clients - 1;
}

void Client_Init(struct Client *client){
    memset(client->firstN,0,20);
    memset(client->lastN,0,20);
    memset(client->username,0,20);
    client->pin = 0;
    client->clientNo = 0;
    client->loanAcctNo = 0;
    client->savAcctNo = 0;
    client->chequeAcctNo = 0;
}
