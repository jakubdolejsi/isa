//
// Created by jakub on 02.10.19.
//



#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <utility>

#define SA struct sockaddr
#define BUF_SIZE 4096

#define CREATE_SOCK_ERR "An error occured while creating socket, check your internet connection\n"
#define CONNECT_SOCK_ERR "Check server availability or rightness of specified ports and try again\n"
#define RECV_SOCK_ERR "An error occured when recieving data from server\n"

#define DUMP_REQUEST(C)\
            cout << " Sending request on " << (C).getHost() << " on port "<< (C).getPort() << " and IP: "<< (C).setIpByHost() << " ..." << endl;\
            cout << " Request: " <<(C).getRequest() <<endl;


#define SOCKET_ERR(P_TEXT, M_TEXT)\
            perror(P_TEXT);\
            fprintf(stderr, M_TEXT);\
            exit(13);\


using namespace std;

class Client {

private:
    string request;
    string host;
    string content;
    int port;
    char *ip;

public:
    Client(string request, string host, int port, string content = "") {
        this->request = move(request);
        this->host = move(host);
        this->port = port;
        this->content = move(content);
    }

    // ------------------------ DUMP METHODS------------------------------------------
    string getRequest() {
        return (this->request);
    }

    string getHost() {
        return (this->host);
    }

    int getPort() {
        return (this->port);
    }
// -----------------------------------------------------------------------------------


    char *setIpByHost() {
        hostent *adr;
        char *ip = nullptr;
        struct in_addr **adressList;
        char *hostname = const_cast<char *>(this->host.c_str());

        if ((adr = gethostbyname(hostname)) == nullptr) {
            exit(40);
        }
        ip = inet_ntoa(*((struct in_addr *) adr->h_addr_list[0]));
        this->ip = ip;

        return (this->ip);
    }

    void connectToServer() {
        string data;
        int sockfd = this->createSocket();

        struct sockaddr_in addr = this->fillStruct();

        this->Connect(sockfd, addr);

        char cstr[this->request.size() + 1];
        strcpy(cstr, this->request.c_str());

        this->sendRequest(sockfd, cstr);

        data = this->Recv(sockfd);
        cout << data << endl;

        close(sockfd);
    }


private:

    string Recv(int sockfd) {
        char buf[BUF_SIZE];
        bzero(buf, BUF_SIZE);
        string data;
        int rc;
        while ((rc = recv(sockfd, buf, BUF_SIZE, 0)) > 0) {
            if (rc < 0) {
                SOCKET_ERR("Reciev err", "An error occured while recieving data")
            }
            data += buf;
            memset(buf, 0, BUF_SIZE);

            return (data);
        }

//        if (recv(sockfd, buf, BUF_SIZE, 0) < 0) {
//            SOCKET_ERR("Recieving socket error: ", RECV_SOCK_ERR);
//        }
        return (buf);
    }

    void sendRequest(int sockfd, char *body) {
        send(sockfd, body, strlen(body), 0);
    }

    sockaddr_in fillStruct() {
        struct sockaddr_in serverAddr;

        bzero(&serverAddr, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(this->port);
        inet_aton(this->ip, reinterpret_cast<in_addr *>(&serverAddr.sin_addr.s_addr)); // naplneni serverAddr.sin_addr

        return (serverAddr);
    }

    int createSocket() {
        int sockfd;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            SOCKET_ERR("Creating socket error", CREATE_SOCK_ERR)
        }
//        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        return (sockfd);
    }

    void Connect(int sockfd, sockaddr_in serverAddr) {
        int conn = connect(sockfd, (SA *) &serverAddr, sizeof(serverAddr));
        if (conn < 0) {
            SOCKET_ERR("Connection error", CONNECT_SOCK_ERR)
        }
    }
};