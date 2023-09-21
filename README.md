# IPC on Linux using Shared Memory & Signals

C++ programs based on the Client-Server architecture that achieve communication between the Server and the multiple Clients through the implementation of Shared Memory and Signals.
- Only one Server process can be running on the system.
- Multiple clients can be run on the system and communicate with the server.
- Clients get a message indicated by the user and send it to the Server.
- The server will save all the received messages in a message log file.
- If a Client is executed and the Server isn't up, the Client will execute the Server immediately.
- The Server will have a counter of how many messages it has received and all Clients can access this count.
- Clients can terminate the execution of the Server
- Two shared memory files are used: one for sending/receiving the messages and the other for Server status data and Client writing indicator. 
- Signals are used to terminate the Server execution and unlink both shared memory files.
## Terminal Personalization
#### Get item
For the Client program be able to correctly execute the Server, the macro TERMINAL needs to be initialized to the terminal of your preference or the one aviliable on your system. If this macro isn't mdoified, by default the Client will try to open the Server in an alacritty terminal resulting in an error if this terminal does not exists in your system.
```cpp
#define TERMINAL "/usr/bin/<terminal>"
```
## Authors

- [@EmilioRivera0](https://github.com/EmilioRivera0)
