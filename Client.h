//
// Created by jakub on 02.10.19.
//

#ifndef ISA_REQUEST_H
#define ISA_REQUEST_H

#endif //ISA_REQUEST_H

#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <arpa/inet.h>


#define DUMP_REQUEST(C)\
            cout << "Sending request on " << C.getHost() << " on port "<< C.getPort() << " and IP: "<< C.getHostByIp() << " ..." << endl;\
            cout <<  C.getRequest() <<endl;


using namespace std;

class Client
{
private:
    string request;
    string host;
    string content;
    int port;
    char *ip;

public:
    Client(string command, string host, int port, string content = "")
    {
        this->request = command;
        this->host = host;
        this->port = port;
        this->content = content;
    }


    string getRequest() {
        return  (this->request);
    }

    string getHost() {
        return (this->host);
    }

    int getPort() {
        return (this->port);
    }


    char *getHostByIp()
    {
        hostent *adr;
        char *ip = nullptr;
        struct in_addr **adressList;
        char *hostname = const_cast<char *>(this->host.c_str());

        if((adr = gethostbyname(hostname)) == nullptr){
            exit(40);
        }
        ip = inet_ntoa(*((struct in_addr*) adr->h_addr_list[0]));
        this->ip = ip;

        return (this->ip);
    }

    void connect() {
        struct sockaddr_in addr = this->fillStruct();
        int sockfd = this->createSocket();
        this->bindSocket(sockfd, addr);
    }
private:
    sockaddr_in fillStruct() {
        struct sockaddr_in addr;
        int s;

        addr.sin_family = AF_INET;
        addr.sin_port = htons(this->port);
        inet_aton(this->ip, reinterpret_cast<in_addr *>(&addr.sin_addr.s_addr));

        return addr;
    }

    int createSocket() {
        int s;
        if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            exit(60);
        }
        return s;
    }

    void bindSocket(int sockfd, sockaddr_in addr) {
        bind(sockfd, (struct sockaddr *)& addr, sizeof(addr));
    }

};