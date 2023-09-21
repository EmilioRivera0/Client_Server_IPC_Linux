// necessary libraries -------->
#include "lib/headers/server.h"

// program execution -------->
int main (int argc, char *argv[]) {
  // program variables
  pid_t pid{0};
  short int count{0};
  char * shmptr{nullptr};
  char * flagptr{nullptr};
  std::ofstream message_log_file{};
 
  // system init
  server_init(flagptr, shmptr, message_log_file, pid);
  // server up and working
  while (true) {
    if (flag_shm_read(flagptr)) {
      shm_read(shmptr, count, message_log_file);
      flag_shm_write(flagptr, pid, count);
    }
  }
  return 0;
}
