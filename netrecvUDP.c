/*
    server.c - interaktiver Netzwerk-Server
*/

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

void err_exit(char *message)
 {
  perror(message);
  exit(1);
 }

int main(int argc, char *argv[])
{
	static char buffer[1000];
	int sock_fd,  port, err, length;
	socklen_t addr_size;
	struct sockaddr_in my_addr, client_addr;
	fd_set input_fdset;

	if ((argc != 2) || (strcmp(argv[1], "-h") == 0))
	{
		fprintf(stderr, "Usage: server port\n");
		return(1);
	}

	if (sscanf(argv[1], "%d", &port) != 1)
	{
		fprintf(stderr, "server: Bad port number.\n");
		return(1);
	}

	/*--- socket() ---*/
	sock_fd = socket(AF_INET,SOCK_DGRAM,0);
	if (sock_fd == -1) err_exit("server: Can't create new socket");
   
	bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    my_addr.sin_port=htons(port);

	printf("port = %d\n", port);


	/*--- bind() ---*/
	err = bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr));
	if (err == -1) err_exit("server: bind() failed");

	printf("bindm returned = %d\n", err);

	while (1)
	{
		addr_size = sizeof(struct sockaddr_in);
	    length = recvfrom(sock_fd, buffer, 256, 0,(struct sockaddr *)&client_addr, &addr_size);
		buffer[length]=0;
		printf("received: %s\n");
	}

	close(sock_fd);
	return(0);

   //int sockfd,n;
   //struct sockaddr_in servaddr,cliaddr;
   //socklen_t len;
   //char mesg[1000];

   //sockfd=socket(AF_INET,SOCK_DGRAM,0);

   //bzero(&servaddr,sizeof(servaddr));
   //servaddr.sin_family = AF_INET;
   //servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   //servaddr.sin_port=htons(2345);
   //bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   //for (;;)
   //{
   //   len = sizeof(cliaddr);
   //   n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
   //   printf("-------------------------------------------------------\n");
   //   mesg[n] = 0;
   //   printf("Received the following:\n");
   //   printf("%s",mesg);
   //   printf("-------------------------------------------------------\n");
   //}
 }
