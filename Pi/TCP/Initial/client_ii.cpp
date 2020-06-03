#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include "PC_Commands.hpp"


using namespace std;

int main()
{
    //	Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }

    //	Create a hint structure for the server we're connecting with
    int port = 7777;
    string ipAddress = "10.148.3.125";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //	Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }
    cout <<"Connection successful." << endl;
    //	While loop:
    char buf[128];
    
    string command = "";
    string clientResp = "";
    bool exit = false;

    while (!exit){
        memset(buf, 0, 128);
        clientResp = "";
        int bytesReceived = recv(sock, buf, 128, 0);
        if (bytesReceived == -1){
            cout << "There was an error getting the command from PC\n";
        }

        else{
            command = string(buf, bytesReceived);
            if (command.compare("getCoordinate")==0) clientResp = getCoordinate();

            else if (command.compare("gotoCenter")==0) clientResp = gotoCenter();

            else if (command.compare(0, 4, "step")==0) clientResp = step(command);

            else if (command.compare("exit")==0){
                exit = true;
                clientResp = "4exit";
            }
            else{
                cout<< "Invalid command received";
                clientResp = "7invalid";
            }
            cout << command << endl;
        }
        
        int sendRes = send(sock, clientResp.c_str(), clientResp.size() + 1, 0);
        if (sendRes == -1){
            cout << "Could not send to PC!\n";
        }
    }
    
    //	Close the socket
    close(sock);

    return 0;
}


