//
// Created by jakub on 02.10.19.
//
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>


#ifndef ISA_SERVER_H
#define ISA_SERVER_H

#endif //ISA_SERVER_H
#define SA struct sockaddr
#define BACKLOG 10 // pocet spojeni
#define BUFF_SIZE 4096
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
        serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);
        serverAddr.sin_port = htons (this->port);

        return (serverAddr);
    }

    int createSocket() {
        int option = 1;
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if((sockfd < 0)) {
            perror("socketError:");
            exit(10);
        }
        cout << "Creating server socket..." << endl;
//        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        return  sockfd;
    }

    void bindSocket(int sockfd, sockaddr_in serverAddr) {

        if(bind(sockfd, (SA *)&serverAddr , sizeof(serverAddr)) < 0) {
            perror("bind Error: ");
            exit(10);
        }
        cout << "Binding server socket..." << endl;
    }


    void Listen(int sockfd) {
        if(listen(sockfd, BACKLOG) < 0) {
            perror("listen eror: ");
            exit(10);
        }
        cout << "Server is listening on port " << this->port << endl;
    }

    int Accept(int sockfd, sockaddr_in clientAddr) {
        socklen_t clientAddrSize = sizeof(clientAddr);

        cout << "Server will accept now..." << endl;
        int acceptedSock = accept(sockfd, (SA *)&clientAddr, &clientAddrSize);

        if(acceptedSock < 0) {
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

        sockfd = this->createSocket();
        serverAddr = this->fillStructure();
        this->bindSocket(sockfd, serverAddr);
        this->Listen(sockfd);

        while (true) {
            acceptSockfd = this->Accept(sockfd, clientAddr);
            printf("server: got connection from %s to port %hu\n", inet_ntoa(clientAddr.sin_addr), clientAddr.sin_port);

            if ((childpid = fork()) == 0) {
                close(sockfd);
                cout << "client socket" << acceptSockfd << endl;
                this->processClientData(acceptSockfd);
                exit(0);
            }
            close(acceptSockfd);
        }
    }


    void processClientData(int clientSock) {
        char buf[BUFF_SIZE];;
        recv(clientSock, buf, BUFF_SIZE, 0);
        cout << buf << endl;
    }



};