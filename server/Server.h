//
// Created by jakub on 14.10.19.
//

#ifndef ISA_SERVER_H
#define ISA_SERVER_H

#include <netinet/in.h>
#include <mutex>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include "Data.h"

using std::mutex;

class Server {

private:
    /**
     * @brief port
     */
    int port;
    /**
     * @brief data
     */
    Data data;


public:

    explicit Server(int port);

    void mainLoop();

private:

    sockaddr_in fillStructure();

    int createSocket();

    void bindSocket(int sockfd, sockaddr_in serverAddr);


    void Listen(int sockfd);

    int Accept(int sockfd, sockaddr_in clientAddr);

    char *createSharedMemory(int &segment_id);

    int *createMutexSharedMemory(int &segment_id, struct shmid_ds &shmbuffer);

    vector<string> parseClientData(int clientSock);

    void Send(int acceptSockfd, const string &data);

    string Recv(int clientSock, int flag);

    string processClientData(vector<string> data);

    void endOfData(int sockfd);

};

#endif //ISA_SERVER_H
