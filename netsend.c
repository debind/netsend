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
		fprintf(stderr, "connect: bad argument '%s'\n",
		argv[2]);
		return(1);
	}

	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
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
		fprintf(stderr, "connect: Bad IP-Address '%s'\n",
				argv[1]);
		return(1);
	}

	err = connect(sock_fd, (struct sockaddr *)&server_addr,
				sizeof(struct sockaddr_in));
	if (err == -1)
	{
		perror("connect: connect() failed");
		return(1);
	}

	argnumber = 3;
	do
	{
		if (sscanf(argv[argnumber], "%s", buffer) == 1)
		{
			argnumber++;
			if (argnumber == argc)
			{
				strcat(buffer, "\n");
			}
			else
			{
				strcat(buffer, " ");
			}
			length = strlen(buffer);
			if (send(sock_fd, buffer, length, 0) == -1)
			{
				perror("send: send() failed");
				close(sock_fd);
				return(1);
			}
		}
		else
		{
			break;
		}
	}
	while(argnumber < argc);

	close(sock_fd);
	return(0);
 }
