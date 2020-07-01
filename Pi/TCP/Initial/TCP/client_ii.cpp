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
// using namespace cv;

int main()
{
    //	Create a socket
    setup();
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }

    //	Create a hint structure for the server we're connecting with
    int port = 7777;
    string ipAddress = "10.49.7.249";

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
    int i = 0;
    string command = "";
    string clientResp = "";
    bool exit = false;
    int numInvalid = 0;
    while (!exit){
        memset(buf, 0, 128);
        clientResp = "";
        int bytesReceived = recv(sock, buf, 128, 0);
        if (bytesReceived == -1){
            cout << "There was an error getting the command from PC\n";
        }

        else{
            command = string(buf, bytesReceived);
            if (command.compare("getCoordinate")==0){
                clientResp = getCoordinate(i);
                i += 1;
            }

            else if (command.compare("gotoCenter")==0) clientResp = gotoCenter();

            else if (command.compare(0, 4, "step")==0) clientResp = step(command);

            else if (command.compare("exit")==0){
                exit = true;
                clientResp = "04exit";
            }
            else{
                numInvalid++;
                cout<< "Invalid command received: " << command << endl;
                clientResp = "07invalid";
                if (numInvalid >= 10) exit = true;
            }
            // cout << command << endl;
        }
        
        int sendRes = send(sock, clientResp.c_str(), clientResp.size() + 1, 0);
        if (sendRes == -1){
            cout << "Could not send to PC!\n";
        }
    }
    run_exit();
    
    //	Close the socket
    close(sock);

    return 0;
}


