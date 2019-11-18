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

#define SA struct sockaddr // makro pro pretypovani struktury
#define BUF_SIZE 4096 // velikost prijimanych dat, stejne jak u serveru

#define CREATE_SOCK_ERR                                                        \
  "An error occured while creating socket, check your internet connection\n"
#define CONNECT_SOCK_ERR                                                       \
  "Check server availability or rightness of specified ports and try again\n"
#define RECV_SOCK_ERR "An error occured when recieving data from server\n"

#define DUMP_REQUEST(C)                                                        \
  cout << " Sending request on " << (C).getHost() << " on port "               \
       << (C).getPort() << " and IP: " << (C).setIpByHost() << " ..." << endl; \
  cout << " Request: " << (C).getRequest() << endl;


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

    // --------------- DUMP methods ---------------------
    string getRequest();

    string getHost();

    int getPort();
    // --------------------------------------------------


    /**
     * @brief Nastavi ip dle hostname
     */
    void setIpByHost();

    /**
     * @brief Probehne pripojeni k serveru a odeslani requestu
     */
    void connectToServer();

private:
    /**
     * @brief wrapper na socket funkci recv
     * @param sockfd socket deskriptor id
     * @return prijata dat
     */
    string Recv(int sockfd);

    /**
     * @brief Odesle request, wrapper na socket funkci send
     * @param sockfd socket deskriptor
     * @param body obsah requestu
     */
    void sendRequest(int sockfd, char* body);

    /**
     * @brief Naplni strukturu sockaddr_in (AF_INET jak rodina adres pro ipv4, port a ip)
     * @return naplnena struktura
     */
    sockaddr_in fillStruct();

    /**
     * @brief Vytvori novy socket(SOCKET_STREAM pro TCP, AF_INET pro ipv4)
     * @return deskriptor socketu - cele cislo
     */
    int createSocket();

    /**
     * @brief wrapper na funkci conncet, preda ji socket deskriptor a skturkturu sockaddr_in pretypovanou na sockaddr
     * @param sockfd socket deskrpitor
     * @param serverAddr struktura pro server data
     */
    void Connect(int sockfd, sockaddr_in serverAddr);

    /**
     * @brief Naparsovani prijatych dat
     * @param response prijata data
     */
    void parseResponse(string response);
};

#endif // ISA_CLIENT_H
