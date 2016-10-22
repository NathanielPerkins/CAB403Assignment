#include "fileControl.h"


int8_t get_file_dimensions(const char *file_location, int16_t *lines, int16_t *width){
    FILE *fp;
    (*lines) = 0;
    (*width) = 0;

    fp = fopen(file_location,"r");
    if (fp == NULL) {
        printf("gfd: Couldn't open file: %s\n",file_location);
        return 1;
    }
    uint64_t buf_size = 300;
    char *buffer = (char*)malloc(sizeof(char)*buf_size);
    int8_t num = 0;
    getline(&buffer,&buf_size,fp); // get header file
    while (getline(&buffer,&buf_size,fp) != -1){
        if (strlen(buffer) > 20) num++;
        if (strlen(buffer) > (uint16_t)*width) (*width) = strlen(buffer);
    }
    (*lines) = num;

    fclose(fp);
    return 0;
}

void open_CD(struct Client *clients, int8_t num_clients, int8_t MAX_WIDTH){
    const char *filename = "Client_Details.txt";
    FILE *fp;
    fp = fopen(filename, "r");
    uint64_t buf_len = MAX_WIDTH;
    if (fp == NULL){
        printf("OCD: Couldn't load %s\n",filename);
    }
    char *buffer = (char*)calloc(MAX_WIDTH,sizeof(char));
    getline(&buffer,&buf_len,fp);
    memset(buffer,0,buf_len);
    for (int8_t i = 0; i< num_clients;i++){
        getline(&buffer,&buf_len,fp);
        char *token = strtok(buffer, " \t");
        memcpy(clients[i].firstN,token,strlen(token));
        token = strtok(NULL, " \t");
        memcpy(clients[i].lastN,token,strlen(token));
        token = strtok(NULL, " \t");
        clients[i].clientNo = strtol(token,NULL,10);
        token = strtok(NULL, " ,\t\n");
        while (token != NULL){
            int32_t acctNo = strtol(token,NULL,10);
            if (acctNo%11==0) clients[i].savAcctNo = acctNo;
            if (acctNo%12==0) clients[i].loanAcctNo = acctNo;
            if (acctNo%13==0) clients[i].chequeAcctNo = acctNo;
            token = strtok(NULL, " ,\t\n");
        }
        memset(buffer,0,buf_len);
    }
    free(buffer);
    fclose(fp);
}
void open_Au(struct Client *clients, int8_t num_clients, int8_t MAX_WIDTH){
    const char *filename = "Authentication.txt";
    FILE *fp;
    fp = fopen(filename, "r");
    uint64_t buf_len = MAX_WIDTH;
    if (fp == NULL){
        printf("OAU: Couldn't load %s\n",filename);
    }
    char *buffer = (char*)calloc(MAX_WIDTH,sizeof(char));
    getline(&buffer,&buf_len,fp);
    memset(buffer,0,buf_len);
    for (int8_t i = 0; i< num_clients;i++){
        getline(&buffer,&buf_len,fp);
        char *token = strtok(buffer, " \t");
        char username[20] = {0};
        memcpy(username,token,strlen(token));
        token = strtok(NULL, " \t");
        int32_t pin = strtol(token,NULL,10);
        token = strtok(NULL, " \t\n");
        int32_t clientNo = strtol(token,NULL,10);
        for (int8_t j = 0;j<num_clients;j++){
            if (clients[i].clientNo == clientNo){
                memcpy(clients[i].username,username,strlen(username));
                clients[i].pin = pin;
                break;
            }
        }
        memset(buffer,0,buf_len);
    }
    free(buffer);
    fclose(fp);
}

void open_Acc(struct Account *accounts, int8_t num_accounts, int8_t MAX_WIDTH){
    const char *filename = "Accounts.txt";
    FILE *fp;
    fp = fopen(filename, "r");
    uint64_t buf_len = MAX_WIDTH;
    if (fp == NULL){
        printf("OACC: Couldn't load %s\n",filename);
    }
    char *buffer = (char*)calloc(MAX_WIDTH,sizeof(char));
    getline(&buffer,&buf_len,fp);
    memset(buffer,0,buf_len);
    for (int8_t i = 0; i< num_accounts;i++){
        getline(&buffer,&buf_len,fp);

        char *token = strtok(buffer," \t");
        accounts[i].accountNo = strtol(token,NULL,10);
        token = strtok(NULL," \t");
        accounts[i].openBal = strtof(token,NULL);
        token = strtok(NULL," \t");
        accounts[i].currentBal = strtof(token,NULL);
        if(accounts[i].accountNo%11 == 0) accounts[i].type = 0;
        if(accounts[i].accountNo%12 == 0) accounts[i].type = 1;
        if(accounts[i].accountNo%13 == 0) accounts[i].type = 2;

        memset(buffer,0,buf_len);
    }
    free(buffer);
    fclose(fp);
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

int8_t username_exists(struct Client *clients,int8_t num_clients,char *username){
    for(int16_t i = 0; i < num_clients; i++){
        if(!strcmp(username,clients[i].username)){
            printf("client[%d]: %s %s requested access\n",i,clients[i].firstN,clients[i].lastN);
            return i;
        }
    }
    return -1;
}

bool pin_correct(struct Client client,int32_t pin){
    return (client.pin == pin);
}
