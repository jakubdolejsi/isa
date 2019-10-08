//
// Created by jakub on 02.10.19.
//
#include <cstdlib>
#include <cerrno>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "RequestParser.h"
#include "Data.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdio>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>


#ifndef ISA_SERVER_H
#define ISA_SERVER_H


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
    Data data;

public:
    Server(int port) {
        this->port = port;
        this->data = Data();
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


        sockfd = this->createSocket();
        serverAddr = this->fillStructure();
        this->bindSocket(sockfd, serverAddr);
        this->Listen(sockfd);

        void* shmem = this->create_shared_memory(128);


        while (true) {
            acceptSockfd = this->Accept(sockfd, clientAddr);

            if (!fork()) {
                string dataToSharedMemory;
                string dataToSend;

                //TODO NEJAK SE TO DIVNE PREPISUJE
                this->data.deserialize(shmem);

                dataToSend = this->processClientData(this->parseClientData(acceptSockfd));

                dataToSharedMemory =this->data.serialize(this->data.getBoards());

                cout << "-------------------" << endl;
                int n = dataToSharedMemory.length();
                char char_array[n + 1];
                strcpy(char_array, dataToSharedMemory.c_str());

                printf("Before insert : %s\n", shmem);
                memcpy(shmem, char_array, sizeof(char_array));
                printf("After Insert: : %s\n", shmem);

                this->Send(acceptSockfd, dataToSend);

                close(sockfd);
                exit(0);
            }
            close(acceptSockfd);
        }

    }


    void* create_shared_memory(size_t size) {
        // Our memory buffer will be readable and writable:
        int protection = PROT_READ | PROT_WRITE;

        // The buffer will be shared (meaning other processes can access it), but
        // anonymous (meaning third-party processes cannot obtain an address for it),
        // so only this process and its children will be able to use it:
        int visibility = MAP_SHARED | MAP_ANONYMOUS;

        // The remaining parameters to `mmap()` are not important for this use case,
        // but the manpage for `mmap` explains their purpose.
        return mmap(NULL, size, protection, visibility, -1, 0);
    }
    vector<string> parseClientData(int clientSock) {
        string recvData;
        vector<string> response;
        // tady
        recvData = this->Recv(clientSock, 0);

        RequestParser requestParser = RequestParser(recvData);
        response = requestParser.process();

        return (response);
    }

    void Send(int acceptSockfd, string data) {
        char newData[data.size() + 1];
        strcpy(newData, data.c_str());    // or pass &s[0]
        if (send(acceptSockfd, newData, strlen(newData), 0) < 0) { // data se do browseru neodislaji vsechna
            SOCKET_ERR("Send error: ", SEND_SOCK_ERR)
        }
    }


    char *stringToChar(string s) {
        char cstr[s.size() + 1];
        strcpy(cstr, s.c_str());
        return cstr;
    }

    string Recv(int clientSock, int flag) {
        char data[BUFF_SIZE];
        bzero(data, BUFF_SIZE);

        if (recv(clientSock, data, BUFF_SIZE, flag) < 0) {
            SOCKET_ERR("Recv", "Recv error occured ");
        }
        string s(data);
        return (s);
    }

    string processClientData(vector<string> data) {
        return (this->data.process(data));
    }


};

#endif //ISA_SERVER_H