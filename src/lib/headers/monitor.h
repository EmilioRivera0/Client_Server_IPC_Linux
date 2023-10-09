#ifndef MONITOR_H
#define MONITOR_H

// necessary libraries -------->
#include <semaphore.h>
#define INCLUDE_ONCE
#include "../../client/lib/headers/client.h"
#include "../../server/lib/headers/server.h"

// macros declaration & definition -------->
#define SEMAPHORE_NAME "/ServerClientSemaphore"
#define SEMAPHORE_SIZE 4
#define SHM_SIZE 100
#define SERVER_SEMAPHORE_SERVER_FLAG O_CREAT|O_EXCL
#define SEMAPHORE_PERMISSIONS S_IRUSR|S_IWUSR

// exceptions declaration & definition -------->
namespace exc{
struct server_semaphore_open_failed : std::exception{
  const char * what() const noexcept{return "Program Failed to Open the Semaphore\n";}
};
struct client_semaphore_open_failed : std::exception{
  const char * what() const noexcept{return "Client Process Failed to Open Semaphore\n";}
};
}

// Monitor class declaration -------->
class Semaphore_Monitor{
private:
  const bool is_server;
  sem_t* sem_ptr;
  char temp_messg[SHM_SIZE]{""};

public:
  Semaphore_Monitor(const bool);
  ~Semaphore_Monitor();
  void read_write(char*, short int&, std::ofstream&);
  void read_write(char*);
};

#endif // !MONITOR_H
