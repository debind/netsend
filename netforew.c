#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <semaphore.h>


#include "main.h"


//--------------------------------------------------
// local function prototypes
void* CommandThread        (void* text);
void  err_exit             (char *message);
void  getcmd               (char* Src, CMD* ptCmd);
int   netsend              (int argc, char *argv[]);
//--------------------------------------------------

#define MAINCYCLE 50

//--------------------------------------------------
// global vars
pthread_t   threadCmd;
char		CmdBuffer[1000] = {0};
sem_t       mutex_CmdBlock; 
int         port;
int         iExit;
int         iCmd;

char        inetadr[100];

char acBuf0[100];
char acBuf1[100];
char acBuf2[100];
char acBuf3[100];
char acBuf4[100];
char acBuf5[100];
char acBuf6[100];
char acBuf7[100];
char acBuf8[100];
char acBuf9[100];
//--------------------------------------------------

//--------------------------------------------------
// main entry
//--------------------------------------------------
int main(int argc, char * argv[])
{
	FILE* fLogFile;
	int   iCount;
	int   status ;
	int*  piStatus;
	CMD   tCmd;


	iExit = 0;
	iCmd = 0;

	tCmd.pBuf[0] = acBuf0;
	tCmd.pBuf[1] = acBuf1;
	tCmd.pBuf[2] = acBuf2;
	tCmd.pBuf[3] = acBuf3;
	tCmd.pBuf[4] = acBuf4;
	tCmd.pBuf[5] = acBuf5;
	tCmd.pBuf[6] = acBuf6;
	tCmd.pBuf[7] = acBuf7;
	tCmd.pBuf[8] = acBuf8;
	tCmd.pBuf[9] = acBuf9;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: netforew ip-addr port \n");
		return(1);
	}

	if (sscanf(argv[2], "%d", &port) != 1)
	{
		fprintf(stderr, "connect: bad argument '%s'\n",
		argv[2]);
		return(1);
	}

	if (sscanf(argv[1], "%s", inetadr) != 1)
	{
		fprintf(stderr, "connect: bad argument '%s'\n",
		argv[1]);
		return(1);
	}

	//-----------------------------------------
	// create semaphore
	sem_init(&mutex_CmdBlock, 0, 1);
	//-----------------------------------------

	// --------------------------------------------------------
	// run the command thread (TCP/IP)
	printf(" starting thread cmd ... ");
	if (pthread_create(&threadCmd, NULL, CommandThread, NULL))
	{
		fprintf(stderr, "pthread: pthread_create() failed.\n");
	}
	// --------------------------------------------------------

	// --------------------------------------------------------
	// welcome to the machine
	while(iExit == 0)
	{
		//------------------------------
		// Commando Verarbeitung
		sem_wait(&mutex_CmdBlock); 
		if(iCmd == 1)
		{
			netsend(tCmd.iCount, tCmd.pBuf);
			getcmd(CmdBuffer, &tCmd);
			iCmd = 0;
		}
		sem_post(&mutex_CmdBlock);
		//------------------------------


		iCount++;

		usleep(MAINCYCLE*1000ul);
	}
	// --------------------------------------------------------

	//--------------------------------------
	// wait for the threads to stop
	pthread_join(threadCmd, (void*)piStatus);
	//--------------------------------------

	sem_destroy(&mutex_CmdBlock);

    return status ;
}


int netsend(int argc, char *argv[])
 {
	static char buffer[256];
	char telegram[1000];
	int sock_fd, err, length, port;
	struct sockaddr_in server_addr;
	fd_set input_fdset;
	int argnumber;


	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		perror("connect: Can't create new socket");
		return(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	err = inet_aton(inetadr, &(server_addr.sin_addr));
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
	telegram[0]=0;
	do
	{
		if (sscanf(argv[argnumber], "%s", buffer) == 1)
		{
			strcat(telegram, buffer);
			argnumber++;
			if (argnumber == argc)
			{
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
	if (send(sock_fd, telegram, length+1, 0) == -1)
	{
		perror("send: send() failed");
		close(sock_fd);
		return(1);
	}

	close(sock_fd);
	return(0);
 }


//******************************************************************************
//get the whole command
//******************************************************************************
void getcmd(char* Src, CMD* ptCmd)
{
	char* pBuf;

	*ptCmd->pBuf[0] = 0;
	*ptCmd->pBuf[1] = 0;
	*ptCmd->pBuf[2] = 0;
	*ptCmd->pBuf[3] = 0;
	*ptCmd->pBuf[4] = 0;
	*ptCmd->pBuf[5] = 0;
	*ptCmd->pBuf[6] = 0;
	*ptCmd->pBuf[7] = 0;
	*ptCmd->pBuf[8] = 0;
	*ptCmd->pBuf[9] = 0;

	pBuf = strtok(Src, " ");

	ptCmd->iCount = 0;

	if (pBuf != NULL) { strcpy(ptCmd->pBuf[0], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[1], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[2], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[3], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[4], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[5], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[6], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[7], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[8], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
	if (pBuf != NULL) { strcpy(ptCmd->pBuf[9], pBuf); ptCmd->iCount++; pBuf = strtok(NULL, " "); }
}


//******************************************************************************
// print error and exit 
//******************************************************************************
void err_exit(char *message)
 {
	perror(message);
	exit(1);
 }


//******************************************************************************
// thread updating the display cyclically
//******************************************************************************
void* CommandThread(void* text)
{
	int         iStatus;
	char*       pBuf;
	char        Trash[200];
	int         nReceived;
	int         sock_fd, client_fd,  err, length;
	socklen_t   addr_size;
	struct      sockaddr_in my_addr, client_addr;
	printf(" ... done\n ");

	/*--- socket() ---*/
	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) err_exit("server: Can't create new socket");

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/*--- bind() ---*/
	err = bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));
	if (err == -1) err_exit("server: bind() failed");

	/*--- listen() ---*/
	err = listen(sock_fd, 1);
	if (err == -1) err_exit("server: listen() failed");

	/*--- accept() ---*/
	while (iExit == 0)
	{
		/*--- accept() ---*/
		addr_size = sizeof(struct sockaddr_in);
		client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addr_size);
		if (client_fd == -1)  err_exit("server: accept() failed");

		pBuf      = CmdBuffer;
		nReceived = 0;
		do
		{
			length     = recv(client_fd, pBuf, 5, 0);
			nReceived += length;
			pBuf      += length;
		}
		while((CmdBuffer[nReceived-1] != 0) && (nReceived < 200));

		while (recv(client_fd, Trash, 100, 0) != 0);


		if (nReceived != 0)
		{
			sem_wait(&mutex_CmdBlock); 
			iCmd = 1;
			while(iCmd == 1)
			{
				sem_post(&mutex_CmdBlock);
				usleep(MAINCYCLE*1000ul);
				sem_wait(&mutex_CmdBlock); 
			}
			sem_post(&mutex_CmdBlock);
		}
	}

	close(client_fd);
	close(sock_fd);

	pthread_exit(&iStatus);
}




