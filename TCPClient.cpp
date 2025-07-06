#include <string>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // For the close() fxn

class TCPClient{
    public:
        TCPClient();
        ~TCPClient();
          
        //Initialize connection to server and return whether a server has connected or not
        bool connectToServer(const std::string& serverIp, int port);
        //Initlailze function for sending a message to server
        void sendMessage(const std::string& sendMessage);
        //Initializa function for recieving a response from server
        std::string recieveResponse();
    
    private:// Only the TCP client itself can access its socket address and the address of the server it is communicating too
        int sock_fd; // Socket file descriptor, number used similar to a name for the socket used by computers
        sockaddr_in server_addr;

};

//COnstructor for creating the TCP client socket
 TCPClient::TCPClient(){
    sock_fd = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
    if (sock_fd < 0){
        //to be replaced with exception later
        perror("socket() failed");
    }
 }

 //Destructor for closing socket
 TCPClient::~TCPClient(){
    close(sock_fd);
 }
//Define function to connect to server
bool TCPClient::connectToServer(const std::string& serverIp, int port){
    //Zero out the struct by using memset to set all memory in the struct to 0
    memset(&server_addr, 0, sizeof(server_addr));
    
    //Fill server address details
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // Convert port number to network bytes foonerm

    //Convert IP address from ip string to binary form
    int result = inet_pton(AF_INET, serverIp.c_str(),&server_addr.sin_addr);
    if (result <= 0){ //catch errors
        perror("inet_pton() failed");
        return false;
    }

    //Connect to server and check for error in connection
    if(connect(sock_fd, (sockaddr*)&server_addr,sizeof(server_addr)<0)){
        perror("connect() failed");
        return false;
    }

    //If operations were successful
    return true;

}

void TCPClient::sendMessage(const std::string& message){
    //send() returns the number of bytes sent after it is invoked
    ssize_t numBytesSent = send(sock_fd,message.c_str(),message.length(),0);
    if (numBytesSent<0){
        perror("send() failed");
    } else if (numBytesSent != message.length()){
        //Will be handled later also
        fputs("send() sent an unexpected number of bytes",stderr);
    }

}

std::string TCPClient::recieveResponse(){
    char buffer[BUFSIZ];
    std::string response = "";
    ssize_t numBytesRcvd;

    //We want to recieve as many bytes as sent. More robust solution would be developed later
    if (numBytesRcvd<0){
        perror("recv() failed");
    } else {
        buffer[numBytesRcvd] = '\0';//Null terminate string response
        response = buffer;
    }
    return response;

}