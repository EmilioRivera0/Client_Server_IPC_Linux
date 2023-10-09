// necessary libraries -------->
#include "../lib/headers/monitor.h"

// program execution -------->
int main (int argc, char *argv[]) {
  // program variables
  pid_t pid{0};
  short int count{0};
  char * shmptr{nullptr};
  char * flagptr{nullptr};
  std::ofstream message_log_file{};
  Semaphore_Monitor sem_monitor(true);
 
  // system init
  server_init(flagptr, shmptr, message_log_file, pid);
  // server up and working
  while (true) {
    // wait for new message, save it and update flag shm
    sem_monitor.read_write(shmptr, count, message_log_file);
    flag_shm_write(flagptr, pid, count);
  }
  return 0;
}
