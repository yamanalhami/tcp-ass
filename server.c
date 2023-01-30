#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>

int ccc;

void Close(int fd);
pid_t Fork(void);
void sig_chld(int signo);
void tostring(char str[], int num);
void operations(char recv[], char op[], int fd,struct sockaddr_in cli);
ssize_t Read(int fd, void *ptr, size_t nbytes);
void Listen(int fd, int backlog);
int Socket(int family, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);

int main(int argc, char** argv)
{
    if(argc != 2)
	{
		printf("You should enter PORT number JUST.\n");
		exit(-1);
	}

    //if(atoi(argv[1]) < 1024 && atoi(argv[1]) > 65535)
     if(atoi(argv[1]) != 5001)
     {printf("enter true port number \n"); exit(-1);}
    ccc = 1;
    int listenfd, connfd;
    pid_t pid;
    socklen_t len;
    struct sockaddr_in cli , srv;
    char recev[150];
    char op[2];

    
    signal( SIGCHLD , sig_chld );

    bzero( &srv , sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(atoi(argv[1]));
    srv.sin_addr.s_addr = htonl(INADDR_ANY);

    /* create tcp socket */
    listenfd = Socket( AF_INET , SOCK_STREAM , 0);
    Bind(listenfd , (struct sockaddr *) &srv , sizeof(srv));
    Listen( listenfd , 100 );

    for(;;)
    {
        len = sizeof(cli);
        if(ccc>5) //check if there is more than 5 client 
        {printf("There is 5 clients , please wait\n");  
	
        while(ccc>5){}} //if there is more than 5 client should be wait  
        connfd = accept(listenfd , (struct sockaddr *) &cli , &len);
        if(connfd < 0) {printf("accept Error\n"); exit(-1);}
        ccc++;
	if( (pid = Fork()) == 0 )
        {
            Close(listenfd);
while(1){
	    bzero( recev , sizeof(recev));
            Read( connfd , op , 2);
            Read( connfd , recev , 150);
	    operations(recev,op,connfd,cli);

	  }
            Close(connfd);
            exit(0);
        }
        Close(connfd);

    }

    return 0;
}

void sig_chld(int signo)
{
	pid_t pid;
	int stat;

	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		//printf("child %d terminated\n", pid);
	}
        ccc--;
	return;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)
	{ printf("Error in Bind().\n"); exit(-1); }
}


int Socket(int family, int type, int protocol)
{
	int n;

	if ( (n = socket(family, type, protocol)) < 0)
	{ printf("Error in Socket(). \n"); exit(-1); }
	return(n);
}

void Listen(int fd, int backlog)
{
	if (listen(fd, backlog) < 0)
	{ printf("Error in Listen().\n"); exit(-1); }
}


void Close(int fd)
{
	if (close(fd) == -1)
	{ printf("Error in Close().\n"); exit(-1); }
}

pid_t Fork(void)
{
	pid_t pid;

	if ( (pid = fork()) == -1)
	{ printf("Error in Fork().\n"); exit(-1); }
	return(pid);
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t n;

	if ( (n = read(fd, ptr, nbytes)) < 0)
	{ printf("Error in Read().\n"); exit(-1); }
	else if(n == 0) exit(0);
	return(n);
}

void tostring(char str[], int num) //function that convert to string
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void operations(char recv[], char op[], int fd,struct sockaddr_in cli)
{
	/*printf("%s\n",op);
	for(int i=0 ; i<strlen(recv) ; i++)
	{
		printf("%c",recv[i]);
	}
	printf("\n");*/
    int *num;
    num = (int*) malloc(sizeof(char));
    memset(num,0,sizeof(num));
    int c=0;
    char *pt;
    pt = strtok (recv,",");
    while (pt != NULL) {
        int a = atoi(pt);
        num[c] = a;
        c++;
        pt = strtok (NULL, ",");
    }

    for(int i=0 ; i < c ; i++)
    	printf("%d ", num[i]);
	printf("\n");

    int odCounter = 0;
    int tdCounter = 0;
    int iii = atoi(op);

        for(int i=0 ; i < c ; i++)
        {
            if(num[i] <= 9)
                odCounter++;
            else tdCounter++;
        }
	int temp1=num[0];
	for(int i=1;i<c;i++){
	if(num[i]>temp1) temp1=num[i];
}
	int temp2=num[0];
	for(int i=1;i<c;i++){
	if(num[i]<temp2) temp2=num[i];
}
char s1[10],s2[10],s3[10],s4[10],s5[10],s6[10];

tostring(s1 , odCounter); //convert odCounter to string and store the result in s1
tostring(s2 , tdCounter);
tostring(s3 , temp1);
tostring(s4 , temp2);
tostring(s5 , temp1-temp2);
tostring(s6 , 'U');

    time_t t;
    time(&t);
    FILE *connlog= fopen("connlog.log","ab+"); //open a log file and creat it if not exist

if(iii == 1){    
fprintf(connlog,"the operation accept and Connection Date&Time : %s , the number of operation is %d and ip from  %s port %d \n",ctime(&t),iii,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
 //printf("from %s,port %d\n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
//printf("%s \n",ctime(&t));
/*printf("number of one digit = %d\n",odCounter);*/
if(write(fd , s1 , strlen(s1)) < 0) {printf("there is error in write \n"); exit(-1);
}
}
else if(iii == 2){    
fprintf(connlog,"the operation accept and Connection Date&Time : %s , the number of operation is %d and ip from  %s port %d \n",ctime(&t),iii,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
//printf("number of two digit = %d\n",tdCounter);
if(write(fd , s2 ,strlen(s2)) < 0) {printf("there is error in write \n");
exit(-1);
}
}
else if(iii==3){
fprintf(connlog,"the operation accept and Connection Date&Time : %s , the number of operation is %d and ip from  %s port %d \n",ctime(&t),iii,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
//printf("%d\n",temp1);
if(write(fd , s3 ,strlen(s3)) < 0) {printf("there is error in write \n");
exit(-1);
}
}

else if(iii==4){
fprintf(connlog,"the operation accept and Connection Date&Time : %s , the number of operation is %d and ip from  %s port %d \n",ctime(&t),iii,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
//printf("%d\n",temp2);
if(write(fd , s4 ,strlen(s4)) < 0) {printf("there is error in write \n");
exit(-1);
}
}
else if (iii==5){
fprintf(connlog,"the operation accept and Connection Date&Time : %s , the number of operation is %d and ip from  %s port %d \n",ctime(&t),iii,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
//printf("%d\n",temp1-temp2);
if(write(fd , s5 ,strlen(s5)) < 0) {printf("there is error in write \n");
exit(-1);
}
}
else if (iii > 5 || iii == 0){
fprintf(connlog,"the operation not accept and Connection Date&Time : %s , the number of operation is %d and ip from  %s port %d \n",ctime(&t),iii,inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
printf("Unsupported Operation\n");
if(write(fd , s6 ,strlen(s6)) < 0) {printf("there is error in write \n");
exit(-1);
}
}
}
