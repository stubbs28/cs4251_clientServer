/*
 *  Threaded Server
 *  by Luke Stubbs
 */
#include <iostream> // std::cout
#include <thread>   // std::thread
#include <chrono>   // std::chrono::seconds
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define PORTNO 4251

void handle_client(int sockfd) {
  char buffer[256];
  bzero(buffer,256);

  if(write(sockfd, "Welcome", 7) < 0)
    printf("[%d]ERROR writing to socket\n", sockfd);

  for(;;) {
    bzero(buffer,256);
    if(read(sockfd, buffer, 255) < 0)
      printf("[%d]ERROR reading from socket\n", sockfd);

    printf("[%d]Message: %s\n", sockfd, buffer);
    if(strcmp("q\n", buffer) == 0)
        break;

    int n;
    if(strcmp("w\n", buffer) == 0) {
      n = write(sockfd, "Weather Info",12);
    } else if (strcmp("t\n", buffer) == 0) {
      n = write(sockfd, "Time Info",9);
    } else {
      n = write(sockfd, "Help Info",9);
    }
    if(n < 0)
      printf("[%d]ERROR writing to socket\n", sockfd);
  }
  close(sockfd);
}

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    printf("ERROR opening socket\n");

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORTNO);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    printf("ERROR on binding\n");

  for(;;) {
    std::cout << "Listening...\n";
    listen(sockfd, 5);
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    std::cout << "Accepted: " << newsockfd << "\n";
    std::thread(handle_client, newsockfd).detach();
  }
  close(sockfd);
  return 0;
}
    
