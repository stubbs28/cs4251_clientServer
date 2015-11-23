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
#include <mutex>

#define PORTNO 4251

std::string REQ_WEATHER = "w\n";
std::string REQ_TIME = "t\n";
std::string REQ_QUIT = "q\n";
std::string HELP = "Usage:\n\t[w] for weather\n\t[t] for time\n\t[q] to quit\n";
std::string WELCOME = "Welcome to Atlanta Weather and Time Server\n" + HELP;
std::string WEATHER = "Today's Weather: ";
std::string TIME = "Current Time: ";
std::string WEATHER_TYPE[] = {"Sunny", "Cloudy", "Rainy", "Stormy"};
std::mutex mtx;

void log(std::string str)
{
  mtx.lock();
  std::cout << str;
  FILE* pfile;
  pfile= fopen("log.txt", "a");
  fputs(str.c_str, pfile);
  fclose(pfile);
  mtx.unlock();
}

/* Gets the Current Date and Time */
const std::string currentDateTime() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y/%m/%d %X", &tstruct);
  return buf;
}

/* Returns a Random Weather Report */
std::string currentWeather() {
  srand(time(NULL));
  int temp = rand() % 170 - 45;
  int wtype = rand() % 4;
  std::cout << wtype;
  return std::to_string(temp) + " deg F and " + WEATHER_TYPE[wtype];
}

/* client session handler */
void handle_client(int sockfd) {
  char buffer[256];
  bzero(buffer,256);

  int run = 1;

  // welcome user, display help, prompt for input
  if(write(sockfd, WELCOME.c_str(), WELCOME.length()) < 0) {
    std::string error = "[" + std::to_string(sockfd) + "] ERROR writing to socket\n";
    log(error);
    run = 0;
  }

  // session loop
  while(run > 0) {
    bzero(buffer,256);
    // get user querry
    if(read(sockfd, buffer, 255) < 0) {
      std::string error = "[" + std::to_string(sockfd) + "] ERROR reading from socket\n";
      log(error);
      break;
    }

    // log user querry
    std::string logstr = "[" + std::to_string(sockfd) + "] Message: " + buffer + "\n";
    log(logstr);

    int n;
    // answer user querry
    if(strcmp(REQ_QUIT.c_str(), buffer) == 0)
      break;
    if(strcmp(REQ_WEATHER.c_str(), buffer) == 0) {
      std::string curweather = WEATHER + currentWeather() + "\n";
      n = write(sockfd, curweather.c_str(), curweather.length());
    } else if (strcmp(REQ_TIME.c_str(), buffer) == 0) {
      std::string curtime = TIME + currentDateTime() + "\n";
      n = write(sockfd, curtime.c_str(), curtime.length());
    } else {
      n = write(sockfd, HELP.c_str(), HELP.length());
    }
    if(n < 0) {
      std::string error = "[" + std::to_string(sockfd) + "] ERROR writing to socket\n";
      log(error);
      break;
    }
  }
  // clean up session
  close(sockfd);
}

int main() {
  // create socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("ERROR opening socket\n");
    exit(0);
  }

  // bind to socket
  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORTNO);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR on binding\n");
    exit(0);
  }

  // main loop
  for(;;) {
    log("[Main] Listening...\n");
    listen(sockfd, 5);
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    // accept new client connection
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    std::string accepted = "[Main] Accepted: " + std::to_string(newsockfd) + "\n";
    log(accepted);
    // handle client session in new thread
    std::thread(handle_client, newsockfd).detach();
  }

  // clean up
  close(sockfd);
  return 0;
}
    
