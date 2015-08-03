/*
    netsend.c - einfacher "txt sender"
*/

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

int main(int argc, char *argv[])
 {
	static char buffer[256];
	char telegram[1000];
	int sock_fd, err, length, port;
	struct sockaddr_in server_addr;
	fd_set input_fdset;
	int argnumber;

	if (argc < 4)
	{
		fprintf(stderr, "Usage: connect ip-addr port 'text'\n");
		return(1);
	}

	if (sscanf(argv[2], "%d", &port) != 1)
	{
		fprintf(stderr, "connect: bad argument '%s'\n", argv[2]);
		return(1);
	}

	// UDP Socket erstellen
	sock_fd=socket(AF_INET,SOCK_DGRAM,0);
	if (sock_fd == -1)
	{
		perror("connect: Can't create new socket");
		return(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	err = inet_aton(argv[1], &(server_addr.sin_addr));
	if (err == 0)
	{
		fprintf(stderr, "connect: Bad IP-Address '%s'\n", argv[1]);
		return(1);
	}

	argnumber = 3;
	telegram[0]=0;
	do
	{
		if (sscanf(argv[argnumber], "%s", buffer) == 1)
		{
			strcat(telegram, buffer);
			argnumber++;
			if (argnumber == argc)
			{
				//strcat(buffer, "\n");
			}
			else
			{
				strcat(telegram, " ");
			}
		}
		else
		{
			break;
		}
	}
	while(argnumber < argc);

	length = strlen(telegram);
	printf("==> %s\n",telegram);
	telegram[length] = 0;

	if (sendto(sock_fd, telegram, length+1,0,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1)
	{
		printf("error \n");
		perror("sendto: sendto() failed");
		close(sock_fd);
		return(1);
	}

	close(sock_fd);

	return(0);
   //int sockfd,n;
   //struct sockaddr_in servaddr,cliaddr;
   //char sendline[1000];
   //char recvline[1000];

   //if (argc != 2)
   //{
   //   printf("usage:  udpcli <IP address>\n");
   //   exit(1);
   //}

   //sockfd=socket(AF_INET,SOCK_DGRAM,0);

   //bzero(&servaddr,sizeof(servaddr));
   //servaddr.sin_family = AF_INET;
   //servaddr.sin_addr.s_addr=inet_addr(argv[1]);
   //servaddr.sin_port=htons(2345);

   //while (fgets(sendline, 10000,stdin) != NULL)
   //{
   //   sendto(sockfd,sendline,strlen(sendline),0, (struct sockaddr *)&servaddr,sizeof(servaddr));
   //   //n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
   //   //recvline[n]=0;
   //   //fputs(recvline,stdout);
   //}

 }
















///* Sample UDP client */
//
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <stdio.h>
//
//int main(int argc, char**argv)
//{
//   int sockfd,n;
//   struct sockaddr_in servaddr,cliaddr;
//   char sendline[1000];
//   char recvline[1000];
//
//   if (argc != 2)
//   {
//      printf("usage:  udpcli <IP address>\n");
//      exit(1);
//   }
//
//   sockfd=socket(AF_INET,SOCK_DGRAM,0);
//
//   bzero(&servaddr,sizeof(servaddr));
//   servaddr.sin_family = AF_INET;
//   servaddr.sin_addr.s_addr=inet_addr(argv[1]);
//   servaddr.sin_port=htons(32000);
//
//   while (fgets(sendline, 10000,stdin) != NULL)
//   {
//      sendto(sockfd,sendline,strlen(sendline),0,
//             (struct sockaddr *)&servaddr,sizeof(servaddr));
//      n=recvfrom(sockfd,recvline,10000,0,NULL,NULL);
//      recvline[n]=0;
//      fputs(recvline,stdout);
//   }
//}