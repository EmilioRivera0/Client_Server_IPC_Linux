// necessary libraries -------->
#include "../headers/monitor.h"

// class methods definition -------->
Semaphore_Monitor::Semaphore_Monitor(const bool server):is_server(server){
  // clear temp_messg
  memset(this->temp_messg, 0, SHM_SIZE);
  try {
    // server constructing object
    if (this->is_server) {
      this->sem_ptr = sem_open(SEMAPHORE_NAME,SERVER_SEMAPHORE_SERVER_FLAG,SEMAPHORE_PERMISSIONS,1);
      if (this->sem_ptr == SEM_FAILED) {
        throw exc::server_semaphore_open_failed();
      }
    }
    // client constructing object
    else{
      this->sem_ptr = sem_open(SEMAPHORE_NAME,0,SEMAPHORE_PERMISSIONS,1);
      if (this->sem_ptr == SEM_FAILED) {
        throw exc::client_semaphore_open_failed();
      }
    }
  }
  catch (const std::exception& e) {
    std::cout << e.what();
    exit(EXIT_FAILURE);
  }
}

Semaphore_Monitor::~Semaphore_Monitor(){
  // close and unlink semaphore
  std::cout << "Semaphore Closed and Unlinked" << std::endl;
  sem_close(this->sem_ptr);
  sem_unlink(SEMAPHORE_NAME);
}

// server calling the method (read operation)
void Semaphore_Monitor::read_write(char* shmptr, short int& count, std::ofstream& message_log_file){
  // local variables
  char string[SHM_SIZE];
  while (true) {
    // clear string
    memset(string, 0, SHM_SIZE);
    // lock semaphore
    sem_wait(this->sem_ptr);
    // read shm
    memcpy(string, shmptr, SHM_SIZE);
    // unlock semaphore
    sem_post(this->sem_ptr);
    if (memcmp(this->temp_messg, string, SHM_SIZE) == 0) {/* no new message */} 
    else{
      std::cout << "New Message" << std::endl;
      // clear temp_messg
      memset(this->temp_messg, 0, SHM_SIZE);
      // save readed message
      memcpy(this->temp_messg, string, SHM_SIZE);
      std::cout << string << std::endl;
      // save message in Message.log
      message_log_file << string << std::endl;
      count++;
      break;
    }
  }
}

// client calling the method (write operation)
void Semaphore_Monitor::read_write(char* shmptr){
  // local variables
  char string[SHM_SIZE];
  // clear string
  memset(string, 0, SHM_SIZE);
  // ignore cin content
  std::cin.get();
  // get user input
  std:: cout << "Message> ";
  std::cin.getline(string, SHM_SIZE);
  // lock semaphore
  sem_wait(this->sem_ptr);
  // clear shm
  memset(shmptr, 0, SHM_SIZE);
  // write message on shm
  memcpy(shmptr, string, SHM_SIZE);
  // unlock semaphore
  sem_post(this->sem_ptr);
}
