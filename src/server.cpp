/*
 *  Threaded Server Thing
 *  by Luke Stubbs
 */
#include <iostream> // std::cout
#include <thread>   // std::thread
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctime>

#define PORTNO 4251

std::string REQ_WEATHER = "w\n";
std::string REQ_TIME = "t\n";
std::string REQ_QUIT = "q\n";

std::string HELP = "Usage:\n\t[w] for weather\n\t[t] for time\n\t[q] to quit\n";
std::string WELCOME = "Welcome to Atlanta Weather and Time Server\n" + HELP;
std::string WEATHER = "Today's Weather: ";
std::string TIME = "Current Time: ";

const std::string currentDateTime() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y/%m/%d %X", &tstruct);
  return buf;
}

/* client session handler */
void handle_client(int sockfd) {
  char buffer[256];
  bzero(buffer,256);

  if(write(sockfd, WELCOME.c_str(), WELCOME.length()) < 0)
    std::cout << "[" << sockfd << "] ERROR writing to socket\n";

  for(;;) {
    bzero(buffer,256);
    if(read(sockfd, buffer, 255) < 0)
      std::cout << "[" << sockfd << "] ERROR reading from socket\n";

    std::cout << "[" << sockfd << "]Message: " << buffer << "\n";
    if(strcmp(REQ_QUIT.c_str(), buffer) == 0)
      break;

    int n;
    if(strcmp(REQ_WEATHER.c_str(), buffer) == 0) {
      n = write(sockfd, WEATHER.c_str(), WEATHER.length());
    } else if (strcmp(REQ_TIME.c_str(), buffer) == 0) {
      std::string curtime = TIME + currentDateTime() + "\n";
      n = write(sockfd, curtime.c_str(), curtime.length());
    } else {
      n = write(sockfd, HELP.c_str(), HELP.length());
    }
    if(n < 0)
      std::cout << "[" << sockfd << "] ERROR writing to socket\n";
  }
  // clean up session
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
    
