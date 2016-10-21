#include "fileControl.h"
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT 12345
#define MAX_CONNECTIONS 10
void  INThandler(int sig);

struct Account *accounts;
struct Client *clients;

int16_t num_clients;
int16_t num_accounts;

void setup_database();

int main(int argc, char *argv[]){
	setup_database();
    int16_t port = PORT;
    int16_t sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
    struct sockaddr_in my_addr;    /* my address information */
    struct sockaddr_in their_addr; /* connector's address information */
    socklen_t sin_size;

    if (argc >= 2) {
        port = strtol(argv[1],NULL,10);
    }
    printf("Starting Server with socket %d\n",port);
    signal(SIGINT,INThandler);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

    my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(port);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
	== -1) {
		perror("bind");
		exit(1);
	}
    if (listen(sockfd, MAX_CONNECTIONS) == -1) {
		perror("listen");
		exit(1);
	}
    printf("ATM ONLINE: \n");

    while(true){
        sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, \
		&sin_size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", \
			inet_ntoa(their_addr.sin_addr));
		if (!fork()) { /* this is the child process */
			if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
				perror("send");
			char username[20];
			char password[20];
			int8_t un_len = recv(new_fd,username, 20,0);
			int8_t pin_len = recv(new_fd,password, 20,0);
			username[un_len] = '\0';
			password[pin_len] = '\0';
			printf("Username: %d|%s\n",(int8_t)strlen(username),username);
			printf("Password: %d|%s\n",(int8_t)strlen(password),password);
			// for all clients check if username + password exist
			// if it does exist, then autheroize logon
			// if not send unauth

			int8_t ind = username_exists(clients,num_clients,username);
			printf("Username index: %d\n",ind);
			if (send(new_fd, "Auth", 4, 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
		}
		close(new_fd);  /* parent doesn't need this */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
    }
}

void  INThandler(int sig)
{
    printf("\n");
    printf("Signal Received: %d\n",sig);
    if(sig == 2){
        printf("Closing the Server\n");
		printf("Freeing Clients\n");
		free(clients);
		// need to write new account balances
		printf("Freeing Accounts\n");
		free(accounts);
		// need to write transaction history to text file
		exit(0);
	}
}

void setup_database(){
	int16_t MAX_WIDTH;
	printf("---------------DATABASE SYSTEM SETUP---------------\n");
	printf("Importing Clients\n");
	const char *CD = "Client_Details.txt";
	get_file_dimensions(CD,&num_clients,&MAX_WIDTH);
	clients = (struct Client*)calloc(num_clients, sizeof(struct Client));
	open_CD(clients,num_clients,MAX_WIDTH);
	printf("Importing Authentication\n");
	const char *Auth = "Authentication.txt";
	get_file_dimensions(Auth,&num_clients,&MAX_WIDTH);
	open_Au(clients,num_clients,MAX_WIDTH);
	printf("Total Clients imported: %d\n",num_clients);
	printf("Importing Accounts\n");
	const char *ACC = "Accounts.txt";
	get_file_dimensions(ACC,&num_accounts,&MAX_WIDTH);
	accounts = (struct Account*)calloc(num_accounts,sizeof(struct Account));
	open_Acc(accounts,num_accounts,MAX_WIDTH);
	printf("Total Acccounts Imported: %d\n",num_accounts);
	printf("-----------------DATABASE IMPORTED-----------------\n\n");
}
