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
			recv(new_fd,username, 20,0);
			recv(new_fd,password, 20,0);
			username[20] = '\0';
			password[20] = '\0';
			printf("Username: %s\n",username);
			printf("Password: %s\n",password);
			if (send(new_fd, "1", 2, 0) == -1)
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
		printf("Freeing Accounts\n");
		free(accounts);
		exit(0);
	}
}

void setup_database(){
	printf("Importing database system\n");
	printf("Importing Clients\n");
	num_clients = open_Clients(&clients);
	printf("Total Clients imported: %d\n",num_clients);
	printf("Importing Authentication\n");
	int8_t temp = open_auth(clients);
	if (temp != num_clients){
		printf("Error: Auth file doesn't match client details\n");
		printf("OA clients: %d\n",temp);
	}
	printf("Importing Accounts\n");
	num_accounts = open_Accounts(&accounts);
	printf("Total Acccounts Imported: %d\n",num_accounts);
}
