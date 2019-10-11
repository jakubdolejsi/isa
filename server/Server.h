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
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include "VectorMapper.h"

#ifndef ISA_SERVER_H
#define ISA_SERVER_H


#define SA struct sockaddr
#define BACKLOG 10 // pocet spojeni
#define BUFF_SIZE 1024
#define SEND_SOCK_ERR "An error occured while sending data\n"
#define TEST_DATA "[Test]\n1. kontent\n2. picaSrac\n:\n[DruhyTest]\n1. pica\n2. picaDve:"


#define SOCKET_ERR(P_TEXT, M_TEXT)\
            perror(P_TEXT);\
            fprintf(stderr, M_TEXT);\
            exit(13);\

using namespace std;


class Server {

private:
    int port;
    VectorMapper vectorMapper;
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
        string dataToSend;
        string dataToSharedMemory;
        int segment_id;
        char *shared_memory;
        struct shmid_ds shmbuffer;
        int segment_size;
        bool first = true;


        shmctl (segment_id, IPC_RMID, 0);
        segment_id = shmget (IPC_PRIVATE, 0x6400,
                             IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

        shared_memory = (char*) shmat (segment_id, nullptr, 0);

        shmctl (segment_id, IPC_STAT, &shmbuffer);
        char *x = TEST_DATA; // nejaka vstupni data..
        sprintf(shared_memory, x, sizeof(x));

        sockfd = this->createSocket();
        serverAddr = this->fillStructure();
        this->bindSocket(sockfd, serverAddr);
        this->Listen(sockfd);

        while (true) {

            acceptSockfd = this->Accept(sockfd, clientAddr);

            int pid = fork();
            if (pid == 0) {
                segment_size  = shmbuffer.shm_segsz;

                this->data.setBoards(this->data.vectorMapper.deserialize(shared_memory, first));
                dataToSend = this->processClientData(this->parseClientData(acceptSockfd));
                dataToSharedMemory =this->data.vectorMapper.serialize(this->data.getBoards());

                int n = dataToSharedMemory.length();
                char char_array[n + 1];
                strcpy(char_array, dataToSharedMemory.c_str());

                sprintf(shared_memory, char_array, sizeof(char_array));
                printf("Stav boardu: %s\n", shared_memory);

                shmdt (shared_memory);

                this->Send(acceptSockfd, dataToSend);
                close(sockfd);
                exit(EXIT_SUCCESS);
            }
            wait(NULL);
            close(acceptSockfd);
        }

    }


    vector<string> parseClientData(int clientSock) {
        string recvData;
        vector<string> response;
        recvData = this->Recv(clientSock, 0);

        RequestParser requestParser = RequestParser(recvData);
        response = requestParser.process();

        return (response);
    }

    void Send(int acceptSockfd, string data) {
        char newData[data.size() + 1];
        strcpy(newData, data.c_str());
        if (send(acceptSockfd, newData, strlen(newData), 0) < 0) {
            SOCKET_ERR("Send error: ", SEND_SOCK_ERR)
        }
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