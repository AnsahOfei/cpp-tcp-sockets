#include <iostream>
#include "TCPClient.cpp" //Will use header in next iteration


// argc is for number of arguments and argv is the array of passed arguments, it is a pointer because it points to the location where
// the arguments are stored in memory as string arrays
int main(int argc, char *argv[]){
    //Test for correct number of arguements
    if (argc != 4){
        std::cerr<<"Usage: " << argv[0] << "<Server IP> <Server Port> <Message>";
        return 1;
    }
    //assign server IP to a variable
    std::string serverIP = argv[1];
    //Convert string number to port
    int port = std::stoi(argv[2]);
    std::string message = argv[3];

    //Create client
    TCPClient client;

    // Connect client to server 
    if (client.connectToServer(serverIP,port)){//If connection was successful
        std::cout << "Successfully connected to server."<< std::endl;

        //Send message
        client.sendMessage(message);
        std::cout <<"Sent: "<< message << std::endl;

        std::string response = client.recieveResponse();
        std::cout << "Recieved: " << response << std::endl;

    } else {
        std::cerr << "Failed to connect to server." << std::endl;
    }

    return 0; //Destructor is called when main function ends

}