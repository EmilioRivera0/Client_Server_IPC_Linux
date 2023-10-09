#ifndef SERVER_H
#define SERVER_H

// necessary libraries -------->
#include <csignal>
#include <sys/mman.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <exception>
#include <cstring>
#include <fstream>
#include <cstdio>

// macros declaration & definition -------->
#define SHM_SIZE 100
#define SHM_NAME "/SHM_CLIENT_SERVER"
#define FLAG_NAME "/FLAG_CLIENT_SERVER"
#define MESSG_LOG_FILE_NAME "Message.log"
#define SHM_FLAGS O_CREAT|O_RDWR
#define SHM_PERMISSIONS S_IRUSR|S_IWUSR
#define MMAP_PROT PROT_READ|PROT_WRITE

// exceptions declaration & definition -------->
namespace exc {
struct shm_creation_fail : std::exception{
  const char * what() const noexcept{return "Program Failed to Create SHM\n";}
};
struct ftruncate_fail : std::exception{
  const char * what() const noexcept{return "Program Failed to Truncate File\n";}
};
struct mmap_fail : std::exception{
  const char * what() const noexcept{return "Program Failed to Map SHM into Memory\n";}
};
struct server_up : std::exception{
  const char * what() const noexcept{return "Server Process is Already Running\n";}
};
struct open_message_log_fail : std::exception{
  const char * what() const noexcept{return "Program Failed to Open/Create Message Log File\n";}
};
}

// functions declaration -------->
// server init 
void server_init(char*&, char*&, std::ofstream&, pid_t&);
void check_flag(char*&);
void create_shm(char*&);
void create_messg_log(std::ofstream&);
// server working
void shm_read(const char*, short int&, std::ofstream&);
void flag_shm_write(char*, pid_t&, const short int&);
// signal handler
void sig_handlr(int);
// power off server
void power_off_server(void);

#endif // !SERVER_H
