#include <string>
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
