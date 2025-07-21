#include "TCPServer.cpp"

int main(int argc, char *argv[]){
    if (argc != 2){
        std::cerr << "Usage: "<< argv[0] << "<Server Port>" << std::endl;
        return 1;
    }

    int port = std::stoi (argv[1]);
    TCPServer server(port);
    server.start();

    return 0;// To end the line
}