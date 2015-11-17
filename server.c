/*
 *  Threaded Server
 *  by Luke Stubbs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 4251   // Port to listen on
#define BACKLOG 10  // Passed to listen()

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void* handle(void* newsock) {
  // send, recv, close
  return NULL;
}

int main(void) {
  
  // Get address info
  struct sockaddr_in server_addr;
  bzero((char *) &server_addr, sizeof(serv_addr));
  server_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT);

  // Create the socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
      error("ERROR opening socket");

  // Bind to the address
  if (bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
      error("ERROR on binding");

  // Listen
  if(listen(sockfd, BACKLOG) == -1)
    error("ERROR on listen");

  // Main loop
  for(;;) {
    size_t client_len = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    int newsockfd = accept(sockfd, (struct sockaddr*) &client_addr, &client_len);

    if(newsockfd < 0)
        error("ERROR on accept");

    printf("Got a connection from %s on port %d\n",
  }

  bzero(buffer, 256);
  n = read(newsockfd, buffer, 255);
  if (n < 0)
      error("ERROR reading from socket");
  printf("Hear is the message: %s\n", buffer);
}
