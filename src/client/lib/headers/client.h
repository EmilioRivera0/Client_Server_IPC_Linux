#ifndef CLIENT_H
#define CLIENT_H

// necessary libraries -------->
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
#include <csignal>

// macros declaration & definition -------->
#define SHM_SIZE 100
#define SHM_NAME "/SHM_CLIENT_SERVER"
#define FLAG_NAME "/FLAG_CLIENT_SERVER"
#define SHM_PERMISSIONS S_IRUSR|S_IWUSR
#define MMAP_PROT PROT_READ|PROT_WRITE
#define TERMINAL "/usr/bin/alacritty"

// exceptions declaration & definition -------->
namespace exc {
struct shm_open_fail : std::exception{
  const char * what() const noexcept{return "Program Failed to Open SHM\n";}
};
struct ftruncate_fail : std::exception{
  const char * what() const noexcept{return "Program Failed to Truncate File\n";}
};
struct mmap_fail : std::exception{
  const char * what() const noexcept{return "Program Failed to Map SHM into Memory\n";}
};
struct fork_fail : std::exception{
  const char * what() const noexcept{return "Program Failed to Create Child Process\n";}
};
struct strtok_fail : std::exception{
  const char * what() const noexcept{return "strtok() Function Failed\n";}
};
}

// functions declaration -------->
// client init 
void client_init(char*&, char*&, pid_t&);
void check_flag(void);
void open_flag(char*&);
void open_shm(char*&);
void start_server(void);
void get_server_pid(char*&, pid_t&);
// client working
void get_server_count(const char*);
void flag_shm_write(char*);
void shm_write(char*);
// power off client
void power_off_client(void);
// interface
char menu(void);

#endif // !CLIENT_H
