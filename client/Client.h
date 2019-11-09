//
// Created by jakub on 14.10.19.
//

#ifndef ISA_CLIENT_H
#define ISA_CLIENT_H

#include "../error/Error.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

class Client {

private:
    string request;
    string host;
    string content;
    int port;
    char* ip;

public:
    Client(string request, string host, int port);

    // ------------------------ DUMP
    // METHODS------------------------------------------
    string getRequest();

    string getHost();

    int getPort();
    // -----------------------------------------------------------------------------------

    char* setIpByHost();

    void connectToServer();

private:
    string Recv(int sockfd);

    void sendRequest(int sockfd, char* body);

    sockaddr_in fillStruct();

    int createSocket();

    void Connect(int sockfd, sockaddr_in serverAddr);
};

#endif // ISA_CLIENT_H
