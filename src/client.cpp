/*
 *  Client Thing 
 *  by Luke Stubbs
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define PORTNO 4251

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cout << "usage " << argv[0] << " hostname";
    exit(0);
  }

  // open socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    std::cout << "ERROR opening socket";
    exit(0);
  }

  // get server address
  struct hostent* server = gethostbyname(argv[1]);
  if (server == NULL) {
    std::cout << "ERROR, no such host";
    exit(0);
  }

  // setup server connection
  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, 
      server->h_length);
  serv_addr.sin_port = htons(PORTNO);

  // connecto to server
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    std::cout << "ERROR connecting";
    exit(0);
  }

  // session loop
  char buffer[256];
  for(;;) {
    bzero(buffer,256);
    // get prompt from server
    if(read(sockfd,buffer,255) < 0) {
      std::cout << "ERROR reading from socket";
      exit(0);
    }

    // get user input and send to server
    std::cout << buffer << "-> ";
    fgets(buffer,255,stdin);
    if(write(sockfd,buffer,strlen(buffer)) < 0) {
      std::cout << "ERROR writing to socket";
      exit(0);
    }

    // did the user quit the session?
    if(strcmp("q\n", buffer) == 0)
        break;
  }
  // clean up
  close(sockfd);
  return 0;
}

