#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>

ssize_t Read(int fd, void *ptr, size_t nbytes);

int main(int argc, char** argv)
{
if(argc != 5)
	{
		printf("You should enter just IP and PORT number \n");
		exit(-1);
	}

	if(atoi(argv[2]) < 1024 && atoi(argv[2]) > 65535) {printf("enter true port number \n"); exit(-1);} //ip should be 5001 but we avoid reserved port

        char recev[100];
	int wait_time = atoi(argv[3]); //time wait insert in arg[3]
	int sockfd;
	struct sockaddr_in srv;
	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	if(sockfd < 0) {printf("error in open socket\n"); exit(-1);}
	bzero(&srv , sizeof(srv));
	srv.sin_family = AF_INET;
	srv.sin_port = htons(atoi(argv[2])); //port take from arg[2]
	if(inet_aton(argv[1] , &srv.sin_addr) <= 0 ) {printf("there is error in your IP\n"); exit(-1);}

	if(connect(sockfd , (struct sockaddr *) &srv, sizeof(srv)) < 0)
	{
		printf("there is error in connection\n");
		exit(-1);
	}	// connect function
	char op[2];
	printf("choose one of the following :\n");
	printf("         _______________ Our services : __________________\n");
	printf("        |                                                 |\n");
	printf("        |     ~ 1 for count numbers that have one digit   |\n");
	printf("        |     ~ 2 for count numbers that have two digit   |\n");
	printf("        |     ~ 3 for max number                          |\n");
	printf("        |     ~ 4 for min number                          |\n");
	printf("        |     ~ 5 for diff between max and min numbers    |\n");
	printf("        |_________________________________________________|\n\n");
	scanf("%[^\n]s", op);

    	FILE* ptr;
    	char *str;

     	str = (char*) malloc(sizeof(char));

    	ptr = fopen(argv[4], "r"); //file insert arg[4]

    	if (NULL == ptr) {
        printf("file can't be opened \n");
    	}
    	while (fgets(str, 100, ptr) != NULL) {
	if(write(sockfd , op , 1) < 0) {printf("there is error in write \n"); exit(-1);}
        usleep(wait_time);
        if(write(sockfd , str , strlen(str)) < 0) {printf("there is error in write \n"); exit(-1);}
        Read( sockfd , recev , 10);
        if(op[0] == '1'){ //check if operation=1
        printf("%s\n", str);
        printf("the number of one digit = %s\n", recev);}//print recev(whats read from server)
//        printf("Please choose other option\n");
 
         else if(op[0] == '2'){//check if operation=2
        printf("%s\n", str);
        printf("the number of two digit = %s\n", recev);}
//        printf("Please choose other option\n");
	 else if(op[0] == '3'){
        printf("%s\n", str);
        printf("the max number = %s\n", recev);}
        
         else if(op[0] == '4'){
        printf("%s\n", str);
        printf("the min number = %s\n", recev);}
        
         else if(op[0] == '5'){
        printf("%s\n", str);
        printf("the diff between max and min  = %s\n", recev);}
	
	else {//if operation not from 1 to 5, print error message(Unsupported Operation)
	printf("Unsupported Operation\n");}
	
	printf("Please choose other option\n");
//        scanf(" %[^\n]s", op);
	bzero(str , sizeof(str));
        bzero(recev , sizeof(recev));
	bzero(op , sizeof(op));
       scanf(" %[^\n]s", op); //insert operation
	usleep(wait_time);
    	}
    return 0;
}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
        ssize_t n;

        if ( (n = read(fd, ptr, nbytes)) < 0) //read from socket (from server and check if there is error)
        { printf("Error in Read().\n"); exit(-1); }
        else if(n == 0) exit(0);
        return(n);
}
