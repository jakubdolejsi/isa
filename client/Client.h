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

#define SA struct sockaddr
#define BUF_SIZE 4096

#define CREATE_SOCK_ERR                                                        \
  "An error occured while creating socket, check your internet connection\n"
#define CONNECT_SOCK_ERR                                                       \
  "Check server availability or rightness of specified ports and try again\n"
#define RECV_SOCK_ERR "An error occured when recieving data from server\n"

#define DUMP_REQUEST(C)                                                        \
  cout << " Sending request on " << (C).getHost() << " on port "               \
       << (C).getPort() << " and IP: " << (C).setIpByHost() << " ..." << endl; \
  cout << " Request: " << (C).getRequest() << endl;

#define SOCKET_ERR(P_TEXT, M_TEXT)                                             \
  perror(P_TEXT);                                                              \
  fprintf(stderr, M_TEXT);                                                     \
  exit(13);


using std::cout;
using std::endl;
using std::string;

class Client {

private:
    string request;
    string host;
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

    void parseResponse(string response);
};

#endif // ISA_CLIENT_H
