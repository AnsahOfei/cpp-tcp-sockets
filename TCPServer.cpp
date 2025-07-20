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
        void handleClient (int clientSocket);
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
    }perror("send() failed");

    //construct local address
    sockaddr_in servAddr;
    memset(&servAddr,0,sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);//listen on all addresses
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

TCPServer::~TCPServer(){
    if (listening_socket_fd >= 0){
        close(listening_socket_fd);
    }
}

void TCPServer::start(){
    for(;;){
        sockaddr_in clntAddr;
        socklen_t clntAddrlen = sizeof(clntAddr);

        //Wait for a client to connect
        //accept() is a blocking call
         
        int client_socket_fd = accept(listening_socket_fd, (sockaddr*)&clntAddr, &clntAddrlen);

        if(client_socket_fd <0){
            perror("accept() failed");
            continue;
        }

        std::cout << "Handling new client...." << std::endl;
        handleClient(client_socket_fd);

    }
}

void TCPServer::handleClient(int clientSocket){
    //This must follow the echo protocol plus our new protocol

    //1. Read message length
    uint32_t netMessageLength;
    if (recv(clientSocket, &netMessageLength,sizeof(netMessageLength),0)!= sizeof(netMessageLength)){
        std::cerr << "Client did not send complete length prefix. "<< std::endl;
        close(clientSocket);
        return;
    }

    uint32_t messageLength = ntohl(netMessageLength);

    //2. Use length to recieve message from client
    std::vector<char> buffer(messageLength);

    if(recv(clientSocket,buffer.data(),messageLength,0) != messageLength){
        std::cerr << "Client did not send complete message. "<< std::endl;
        close(clientSocket);
        return;

    }

    std::string recievedMsg(buffer.begin(),buffer.end());
    std::cout<< "Client sent: "<<recievedMsg << std::endl;

    // Echo back to client
    if(send(clientSocket,&netMessageLength,sizeof(netMessageLength),0)!=messageLength){
        perror("send() length failed");

    }else{
        if(send(clientSocket,recievedMsg.c_str(),sizeof(netMessageLength),0)!=sizeof(netMessageLength)){
            perror("send() message failed");

        }
    }
    std::cout<<"Echo sent. Closing client connection." <<std::endl;
    close(clientSocket);
}
