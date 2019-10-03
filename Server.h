//
// Created by jakub on 02.10.19.
//
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "RequestParser.h"


#ifndef ISA_SERVER_H
#define ISA_SERVER_H

#endif //ISA_SERVER_H
#define SA struct sockaddr
#define BACKLOG 10 // pocet spojeni
#define BUFF_SIZE 1024
#define SEND_SOCK_ERR "An error occured while sending data\n"


#define SOCKET_ERR(P_TEXT, M_TEXT)\
            perror(P_TEXT);\
            fprintf(stderr, M_TEXT);\
            exit(13);\

using namespace std;

class Server {

private:
    int port;

public:
    Server(int port) {
        this->port = port;
    }

private:

    sockaddr_in fillStructure() {
        struct sockaddr_in serverAddr;
        bzero(&serverAddr, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(this->port);

        return (serverAddr);
    }

    int createSocket() {
        int option = 1;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if ((sockfd < 0)) {
            perror("socketError:");
            exit(10);
        }
//        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        return sockfd;
    }

    void bindSocket(int sockfd, sockaddr_in serverAddr) {

        if (bind(sockfd, (SA *) &serverAddr, sizeof(serverAddr)) < 0) {
            perror("bind Error: ");
            exit(10);
        }
    }


    void Listen(int sockfd) {
        if (listen(sockfd, BACKLOG) < 0) {
            perror("listen eror: ");
            exit(10);
        }
    }

    int Accept(int sockfd, sockaddr_in clientAddr) {
        socklen_t clientAddrSize = sizeof(clientAddr);

        int acceptedSock = accept(sockfd, (SA *) &clientAddr, &clientAddrSize);

        if (acceptedSock < 0) {
            perror("accept error: ");
            exit(10);
        }
        return (acceptedSock);
    }

public:
    void mainLoop() {
        sockaddr_in serverAddr, clientAddr;
        int sockfd, acceptSockfd;
        pid_t childpid;
        string data;

        sockfd = this->createSocket();
        serverAddr = this->fillStructure();
        this->bindSocket(sockfd, serverAddr);
        this->Listen(sockfd);

        while (true) {
            acceptSockfd = this->Accept(sockfd, clientAddr);
//            printf("server: got connection from %s to port %hu\n", inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);

            if (!fork()) {
                data = this->processClientData(acceptSockfd);
                this->Send(acceptSockfd, data);
                close(sockfd);
                exit(0);
            }
            close(acceptSockfd);
        }
    }
    string processClientData(int clientSock) {
        char data[BUFF_SIZE];
        string response, recvData;
        // tady
        recvData = this->Recv(clientSock, data, 0);

        cout << "Accepted request: " << recvData << endl;
        RequestParser requestParser = RequestParser(data);
        response = requestParser.process();
        return (response);
    }

    void Send(int acceptSockfd, string data) {
        char newData[data.size() + 1];
        strcpy(newData, data.c_str());	// or pass &s[0]

        if (send(acceptSockfd, newData, strlen(newData), 0) < 0) {
            SOCKET_ERR("Send error: ", SEND_SOCK_ERR)
        }
    }


    char *stringToChar(string s) {
        char cstr[s.size() + 1];
        strcpy(cstr, s.c_str());
        return cstr;
    }

    char *Recv(int clientSock, char *data, int flag) {
        if(recv(clientSock, data, BUFF_SIZE, flag) < 0) {
            SOCKET_ERR("Recv", "Recv error occured ");
        }
        return (data);
    }

};