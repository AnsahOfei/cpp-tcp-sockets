#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>

class TCPServer{
    private:
        int listening_socket_fd, port;
        void handleCLient (int clientSocket);
    public:
        TCPServer(int port);
        ~TCPServer();
        void start();
    

};