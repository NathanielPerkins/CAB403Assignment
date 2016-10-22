#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 12345    /* the port client will be connecting to */

#define MAXDATASIZE 100 /* max number of bytes we can get at once */

void logon(int16_t sockfd);
void main_menu(int16_t sockfd);

int main(int argc, char *argv[])
{
	int16_t sockfd, numbytes, port;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	if (argc != 3) {
		fprintf(stderr,"usage: client hostname port\n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
		herror("gethostbyname");
		exit(1);
	}
	port = strtol(argv[2],NULL,10);
	// setup socket information
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(port);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, \
	sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}

	// Enter communication mode
	if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	logon(sockfd);
	main_menu(sockfd);

	close(sockfd);

	return 0;
}

void logon(int16_t sockfd){
	char username[20];
	char password[20];
	char auth[20];
	int8_t str_len;
	printf("=========================================================\n");
	printf("\n       Welcome to PatelPerkins Online Banking\n\n");
	printf("=========================================================\n");
	printf("\nPlease input your Username ----> ");
	scanf("%s",username);
	printf("Please input your Pin      ----> ");
	scanf("%s",password);
	if(send(sockfd,username,20,0) == -1){
		printf("Error sending username\n");
	}
	if(send(sockfd,password,20,0) == -1){
		printf("Error sending password\n");
	}

	if((str_len = recv(sockfd,auth,20,0)) == -1){
		printf("Error receiving authentication token\n");
	}
	auth[str_len] = '\0';
	// int8_t test;
	if(strcmp(auth, "Auth") != 0){
		printf("Password or username is incorrect: Closing Client\n");
		exit(EXIT_FAILURE); // close the client with unsuccessful logon
	}
}
void main_menu(int16_t sockfd){
	char buffer[MAXDATASIZE];
	int8_t str_len;
	printf("\nWelcome to the ATM system\n");
	if((str_len = recv(sockfd,buffer,MAXDATASIZE,0)) == -1){
		printf("Error receiving name\n");
	}
	buffer[str_len] = '\0';
	printf("You are logged on as %s\n",buffer);
	if((str_len = recv(sockfd,buffer,MAXDATASIZE,0)) == -1){
		printf("Error receiving Client Number\n");
	}
	buffer[str_len] = '\0';
	printf("Client Number: %d\n",(int32_t)strtol(buffer,NULL,10));
	printf("Please Enter a selection\n");
	printf("<1> Account Balance\n");
	printf("<2> Withdrawl\n");
	printf("<3> Deposit\n");
	printf("<4> Transfer\n");
	printf("<5> Transaction Listing\n");
	printf("<6> EXIT\n");
	printf("Selection |1-6|: ");
	char selection;
	getchar();
	selection = getchar();
	while(selection < '0' || selection > '6'){
		printf("Please enter a valid selection: ");
		selection = getchar();
		getchar();

	}

}
