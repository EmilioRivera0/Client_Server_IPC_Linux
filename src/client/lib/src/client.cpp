// necessary libraries -------->
#include "../headers/client.h"

// functions definition -------->
void client_init(char*& flagptr, char*&shmptr, pid_t& server_pid){
  std::cout << "Start Up..." << std::endl;
  try {
    // check if server is up and run it if not
    check_flag();
    // open flag shm
    open_flag(flagptr);
    // open shm
    open_shm(shmptr);
    // get server pid
    get_server_pid(flagptr, server_pid);
  }
  catch (const std::exception& e) {
    std::cout << e.what() << std::endl;    
    exit(EXIT_FAILURE);
  }
}

void check_flag(){
  // server process is not up
  if (shm_open(FLAG_NAME, O_RDWR, SHM_PERMISSIONS) == -1){
    start_server();
    // sleep to let server boot up
    usleep(500000);
  }
}

void open_flag(char*& flagptr){
  // local variables
  short int fdescriptor = shm_open(FLAG_NAME, O_RDWR, SHM_PERMISSIONS);
  // server process is not up
  if (fdescriptor == -1)
    throw exc::shm_open_fail();

  std::cout << SHM_NAME << " Successfuly Opened" << std::endl;
  // assign size to flag shm
  if (ftruncate(fdescriptor, SHM_SIZE) == -1)
    throw exc::ftruncate_fail();

  // map shm into process memory and get its address
  flagptr = (char*) mmap(NULL, SHM_SIZE, MMAP_PROT, MAP_SHARED, fdescriptor, 0);
  if (flagptr == MAP_FAILED)
    throw exc::mmap_fail();
  // close file descriptor since it is not needed any more
  close(fdescriptor);
}

void open_shm(char*& shmptr){
  // local variables
  short int fdescriptor = shm_open(SHM_NAME, O_RDWR, SHM_PERMISSIONS);
  // if shm_open fails
  if (fdescriptor == -1)
    throw exc::shm_open_fail();

  std::cout << SHM_NAME << " Successfuly Opened" << std::endl;

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

void start_server(){
  //local variables
  pid_t pid{-1};
  // create child process
  pid = fork();
  // fork failed
  if (pid < 0)
    throw exc::fork_fail();
  // child process
  else if (pid == 0) {
    execl(TERMINAL, TERMINAL, "-e", "../server/Server");
  }
  else
    std::cout << "Server Process Started" << std::endl;
}

void get_server_pid(char*& flagptr, pid_t& server_pid){
  // local variables
  char string[SHM_SIZE]{""};
  char* tokptr{nullptr};
  // read flag shm
  memcpy(string, flagptr, SHM_SIZE);
  // get first token containing server pid
  tokptr = strtok(string, " ");
  // check for errors
  if (tokptr == nullptr)
    throw exc::strtok_fail();
  // initialize server_pid
  server_pid = atoi(tokptr);
  std::cout << "Server PID " << server_pid << std::endl;
}

void get_server_count(const char*flagptr){
  // local variables
  char string[SHM_SIZE]{""};
  char* tokptr{nullptr};
  short int count{0};
  // read flag shm
  memcpy(string, flagptr, SHM_SIZE);
  // get second token containing server count
  tokptr = strtok(string, " ");
  tokptr = strtok(nullptr, " ");
  count = atoi(tokptr);
  std::cout << "Server Message Count " << count << std::endl;
}

void flag_shm_write(char* flagptr){
  // local variables
  char string[SHM_SIZE]{"W"};
  // clear flag shm
  memset(flagptr, 0, SHM_SIZE);
  // write message on shm
  memcpy(flagptr, string, SHM_SIZE);
}

void shm_write(char* shmptr){
  // local variables
  char string[SHM_SIZE]{""};
  // ignore cin content
  std::cin.get();
  // get user input
  std:: cout << "Message> ";
  std::cin.getline(string, SHM_SIZE);
  // clear flag shm
  memset(shmptr, 0, SHM_SIZE);
  // write message on shm
  memcpy(shmptr, string, SHM_SIZE);
}

void power_off_client(){
  exit(EXIT_SUCCESS);
}

char menu(){
  // local variable
  char op{'0'};
  std::cout << "\t<-Client->\n" << std::endl;
  std::cout << "1. Send Message\n2. Close Server\n0. Exit" << std::endl;
  std:: cout << "Option> ";
  std::cin >> op;
  return op;
}
