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

int8_t logon(int16_t sockfd);

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

	if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	logon(sockfd);

	buf[numbytes] = '\0';

	printf("Received: %s",buf);

	close(sockfd);

	return 0;
}

int8_t logon(int16_t sockfd){
	char username[20];
	char password[20];
	printf("=========================================================\n");
	printf("\n       Welcome to PatelPerkins Online Banking\n\n");
	printf("=========================================================\n");
	printf("\nPlease input your Username ----> ");
	scanf("%s",username);
	printf("Please input your Password ----> ");
	scanf("%s",password);
	send(sockfd,username,20,0);
	send(sockfd,password,20,0);

	return 1;
}
