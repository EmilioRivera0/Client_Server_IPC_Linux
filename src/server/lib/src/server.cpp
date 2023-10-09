// necessary libraries -------->
#include "../headers/server.h"

// functions definition -------->
void server_init(char*& flagptr, char*& shmptr, std::ofstream& message_log_file, pid_t& pid){
  // set user defined handler
  struct sigaction sig_act;
  sig_act.sa_handler = sig_handlr;
  sigaction(SIGUSR1, &sig_act, nullptr);

  std::cout << "Start Up..." << std::endl;
  // initialize pid
  pid = getpid();
  std::cout << "PID " << pid << std::endl;
  try {
    // check for flag shm 
    check_flag(flagptr);
    // create shm
    create_shm(shmptr);
    // create message log file
    create_messg_log(message_log_file);
    // initialize flag shm
    flag_shm_write(flagptr, pid, 0);
  }
  catch (const std::exception& e) {
    std::cout << e.what();
    exit(EXIT_FAILURE);
  }
}

void check_flag(char*& flagptr){
  // local variables
  short int fdescriptor = shm_open(FLAG_NAME, O_RDWR, SHM_PERMISSIONS);
  // no server process is running already
  if (fdescriptor == -1) {
    fdescriptor = shm_open(FLAG_NAME, SHM_FLAGS, SHM_PERMISSIONS);
    std::cout << "Flag SHM Created as " << FLAG_NAME << std::endl;
    // assign size to flag shm
    if (ftruncate(fdescriptor, SHM_SIZE) == -1)
      throw exc::ftruncate_fail();
    // map flag shm into process memory and get its address
    flagptr = (char*) mmap(NULL, SHM_SIZE, MMAP_PROT, MAP_SHARED, fdescriptor, 0);
    if (flagptr == MAP_FAILED)
      throw exc::mmap_fail();
    // close file descriptor since it is not needed any more
    close(fdescriptor);
  }
  // server is already up
  else
    throw exc::server_up();
}

void create_shm(char*& shmptr){
  // local variables
  short int fdescriptor = shm_open(SHM_NAME, SHM_FLAGS, SHM_PERMISSIONS);
  // if shm_open fails
  if (fdescriptor == -1)
    throw exc::shm_creation_fail();

  std::cout << "Shared Memory Creaded as " << SHM_NAME << std::endl;

  // assign size to shm
  if (ftruncate(fdescriptor, SHM_SIZE) == -1)
    throw exc::ftruncate_fail();

  // map shm into process memory and get its address
  shmptr = (char*) mmap(NULL, SHM_SIZE, MMAP_PROT, MAP_SHARED, fdescriptor, 0);
  if (shmptr == MAP_FAILED)
    throw exc::mmap_fail();
  // close file descriptor since it is not needed any more
  close(fdescriptor);
}

void create_messg_log(std::ofstream& message_log_file){
  // try to open file
  message_log_file.open(MESSG_LOG_FILE_NAME);
  if (message_log_file.is_open())
    std::cout << "Message Log File Successfuly Open/Created\n" << std::endl;
  else
    throw exc::open_message_log_fail();

}

void shm_read(const char* shmptr, short int& count, std::ofstream& message_log_file){
  // local variables
  char string[SHM_SIZE]{""};
  // read shm
  memcpy(string, shmptr, SHM_SIZE);
  std::cout << string << std::endl;
  message_log_file << string << std::endl;
  count++;
}

void flag_shm_write(char* flagptr, pid_t& pid, const short int& count){
  // local variables
  char string[SHM_SIZE]{""};
  // clear flag shm
  memset(flagptr, 0, SHM_SIZE);
  // set server message
  snprintf(string, SHM_SIZE, "%u %u", pid, count);
  // write message on shm
  memcpy(flagptr, string, SHM_SIZE);
}

void sig_handlr(int num){
  power_off_server();
}

void power_off_server(){
  shm_unlink(SHM_NAME);
  std::cout << "\nShared Memory " << SHM_NAME << " Unlinked" << std::endl;
  shm_unlink(FLAG_NAME);
  std::cout << "Flag Shared Memory " << FLAG_NAME << " Unlinked" << std::endl;
  exit(EXIT_SUCCESS);
}
