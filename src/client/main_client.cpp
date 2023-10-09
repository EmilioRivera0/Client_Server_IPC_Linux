// necessary libraries -------->
#include "../lib/headers/monitor.h"

// program execution -------->
int main (int argc, char *argv[]) {
  // program variables
  pid_t server_pid{0};
  char * flagptr{nullptr};
  char * shmptr{nullptr};
  char op{'0'};

  // system init
  client_init(flagptr, shmptr, server_pid);
  Semaphore_Monitor sem_monitor(false);
  // client up and working
  while (true) {
    op = menu();
    switch (op) {
      // send message
      case '1':
        sem_monitor.read_write(shmptr);
        usleep(500000);
        get_server_count(flagptr);
        break;
      // close server
      case '2':
        kill(server_pid, SIGUSR1);
        sem_monitor.~Semaphore_Monitor();
        power_off_client();
        break;
      // close client
      case '0':
        power_off_client();
        break;
      default:
        std::cout << "Invalid Input" << std::endl;
        break;
    }
  }
  return 0;
}
