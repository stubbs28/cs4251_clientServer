/*
 *  Threaded Server
 *  by Luke Stubbs
 */
#include <iostream> // std::cout
#include <thread>   // std::thread
#include <chrono>   // std::chrono::seconds

void pause_thread(int n) {
  std::this_thread::sleep_for(std::chrono::seconds(n));
  std::cout << "pause of " << n << " seconds ended\n";
}

int main() {
  std::cout << "Spawning and detaching 5 threads...\n";
  for(int i = 0; i < 5; i++) {
    std::thread(pause_thread, i + 1).detach();
  }
  std::cout << "Done spawning threads.\n";
  std::cout << "(the main thread will now pause for 10 seconds)\n";
  pause_thread(10);
  return 0;
}
