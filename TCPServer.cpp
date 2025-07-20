#include <string>
#include <string.h>
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

TCPServer::TCPServer(int port){
    //incoming connections
    listening_socket_fd = socket(AF_INET,SOCK_STREAM,IP_PROTOCOL);
    if (listening_socket_fd < 0){
        perror("socket() failed");
        exit(EXIT_FAILURE);        
    }

    //construct local address
    sockaddr_in servAddr;
    memset(&servAddr,0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    //Bind and listen
    if(bind(listening_socket_fd, (sockaddr*)&servAddr, sizeof(servAddr)) < 0){
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    const int MAX_PENDING_CONNECTIONS = 5;
    if(listen(listening_socket_fd,MAX_PENDING_CONNECTIONS)<0){
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port" << port << std::endl;
}
