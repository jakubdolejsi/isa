//
// Created by jakub on 02.10.19.
//



#include "Server.h"
#include "../Helpers/VectorMapper.h"
#include "RequestParser.h"
#include "../error/Error.h"
#include <iostream>
#include "../Helpers/SignalHandler.h"

Server::Server(int port)
{
    this->port = port;
    this->dataProcesser = DataProcesser();
}

void Server::mainLoop()
{

    sockaddr_in serverAddr, clientAddr;
    int sockfd, acceptSockfd;
    string dataToSend;
    string dataToSharedMemory;
    bool first = true;
    int sharedMemoryId;
    char* sharedMemory;
    int mutexId;
    sem_t* mutex;


    sockfd = this->createSocket();
    serverAddr = this->fillStructure();
    this->bindSocket(sockfd, serverAddr);
    this->Listen(sockfd);

//    shmctl(sharedMemoryId,IPC_RMID,nullptr);
//    shmctl(mutexId,IPC_RMID,nullptr);
    sharedMemory = this->createDataSharedMemory(sharedMemoryId);
    mutex = this->createMutexSharedMemory(mutexId);

    SignalHandler::setSharedMemoryId(sharedMemoryId);
    SignalHandler::setMutexId(mutexId);

    //TODO Testovaci data...
    char* x = const_cast<char*>(TEST_DATA);
    sprintf(sharedMemory, x, sizeof(x));

    sem_init(mutex, 1, 1);
    while (true) {

        acceptSockfd = this->Accept(sockfd, clientAddr);

        int pid = fork();
        if (pid==0) {
            close(sockfd);

            sem_wait(mutex);
            this->dataProcesser.setBoards(
                    VectorMapper::deserialize(sharedMemory, first));
            dataToSend = this->processClientData(this->parseClientData(acceptSockfd));
            dataToSharedMemory =
                    VectorMapper::serialize(this->dataProcesser.getBoards());

            char char_array[dataToSharedMemory.length()+1];
            strcpy(char_array, dataToSharedMemory.c_str());
            sprintf(sharedMemory, char_array, sizeof(char_array));
            sem_post(mutex);

            shmdt(sharedMemory);

            this->Send(acceptSockfd, dataToSend);

            close(acceptSockfd);
            exit(EXIT_SUCCESS);
        }
        waitpid(pid, nullptr, 0);
        close(acceptSockfd);
    }
}

sockaddr_in Server::fillStructure()
{
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(this->port);

    return (serverAddr);
}

int Server::createSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ((sockfd<0)) {
        SOCKET_ERR("Creating socket error", CREATE_SOCK_ERR)
    }
    return sockfd;
}

void Server::bindSocket(int sockfd, sockaddr_in serverAddr)
{

    if (bind(sockfd, (SA*) &serverAddr, sizeof(serverAddr))<0) {
        perror("bind Error: ");
        exit(10);
    }
}

void Server::Listen(int sockfd)
{
    if (listen(sockfd, BACKLOG)<0) {
        perror("listen eror: ");
        exit(10);
    }
}

int Server::Accept(int sockfd, sockaddr_in clientAddr)
{
    socklen_t clientAddrSize = sizeof(clientAddr);

    int acceptedSock = accept(sockfd, (SA*) &clientAddr, &clientAddrSize);

    if (acceptedSock<0) {
        perror("accept error: ");
        exit(10);
    }
    return (acceptedSock);
}

char* Server::createDataSharedMemory(int& sharedMemoryId)
{
    int sharedMemoryKey = ftok("isaclient", 65);
    // Vytvoreni sdileneho segmentu pameti o velikosti
    sharedMemoryId = shmget(sharedMemoryKey, 1024, 0777 | IPC_CREAT);
    if (sharedMemoryId<0) {
        perror("Error while creating shared memory: ");
        exit(1);
    }
    return (char*) shmat(sharedMemoryId, nullptr, 0);
}

sem_t* Server::createMutexSharedMemory(int& mutexId)
{
    int sharedMemoryKey = ftok("isaserver", 65);
    // Vytvoreni sdileneho segmentu pameti o velikosti
    mutexId = shmget(sharedMemoryKey, 1024, 0777 | IPC_CREAT);
    if (mutexId<0) {
        perror("Error while creating shared memory: ");
        exit(1);
    }
    return (sem_t*) shmat(mutexId, nullptr, 0);
}

vector<string> Server::parseClientData(int acceptSockfd)
{
    string recvData;
    vector<string> response;
    recvData = this->Recv(acceptSockfd);

    RequestParser requestParser = RequestParser(recvData);
    response = requestParser.process();

    return (response);
}

void Server::Send(int acceptSockfd, const string& data)
{
    char newData[data.size()+1];
    strcpy(newData, data.c_str());
    if (send(acceptSockfd, newData, strlen(newData), 0)<0) {
        SOCKET_ERR("Send error: ", SEND_SOCK_ERR)
    }
}

string Server::Recv(int clientSock)
{
    char data[BUFF_SIZE];
    bzero(data, BUFF_SIZE);
    int recData = recv(clientSock, data, BUFF_SIZE, 0);

    if (recData<0) {
        SOCKET_ERR("Recv", "Recv error occured ")
    }
    else if (recData==0) {
        std::cout << "adadad" << std::endl;
    }
    //    string s(dataProcesser);
    return (string(data));
}

string Server::processClientData(vector<string> data)
{
    return (this->dataProcesser.process(move(data)));
}

void Server::intHandler()
{
    std::cout << "Port is: " << this->port << std::endl;
}

