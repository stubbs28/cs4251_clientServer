#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define PORTNO 4251

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
     fprintf(stderr,"usage %s hostname\n", argv[0]);
     exit(0);
  }
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
      error("ERROR opening socket");

  struct hostent* server = gethostbyname(argv[1]);
  if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
  }

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, 
      server->h_length);
  serv_addr.sin_port = htons(PORTNO);

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");

  char buffer[256];
  for(;;) {
    bzero(buffer,256);
    if(read(sockfd,buffer,255) < 0)
       error("ERROR reading from socket");
    printf("%s\n",buffer);
    printf("-> ");
    fgets(buffer,255,stdin);
    if(write(sockfd,buffer,strlen(buffer)) < 0)
       error("ERROR writing to socket");
    if(strcmp("q\n", buffer) == 0)
        break;
  }
  close(sockfd);
  return 0;
}

