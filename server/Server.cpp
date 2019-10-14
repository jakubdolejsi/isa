//
// Created by jakub on 02.10.19.
//

#include "Server.h"
#include "../Helpers/VectorMapper.h"
#include "RequestParser.h"


#define SA struct sockaddr
#define BACKLOG 10 // pocet spojeni
#define BUFF_SIZE 1024
#define SEND_SOCK_ERR "An error occured while sending data\n"
#define TEST_DATA "[Test]\n1. kontent\n2. picaSrac\n:\n[DruhyTest]\n1. pica\n2. picaDve\n:"


#define SOCKET_ERR(P_TEXT, M_TEXT)\
            perror(P_TEXT);\
            fprintf(stderr, M_TEXT);\
            exit(13);\


Server::Server(int port) {
    this->port = port;
    this->data = Data();
}


void Server::mainLoop() {
    sockaddr_in serverAddr, clientAddr;
    mutex mtx;
    int sockfd, acceptSockfd;
    string dataToSend;
    string dataToSharedMemory;
    int segment_id;
    char *shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    bool first = true;


    shared_memory = this->createSharedMemory(segment_id);

    shmctl(segment_id, IPC_STAT, &shmbuffer);
    char *x = const_cast<char *>(TEST_DATA); // nejaka vstupni data..
    sprintf(shared_memory, x, sizeof(x));

    sockfd = this->createSocket();
    serverAddr = this->fillStructure();
    this->bindSocket(sockfd, serverAddr);
    this->Listen(sockfd);

    while (true) {

        acceptSockfd = this->Accept(sockfd, clientAddr);

        int pid = fork();
        if (pid == 0) {

            mtx.lock();
            this->data.setBoards(VectorMapper::deserialize(shared_memory, first));
            dataToSend = this->processClientData(this->parseClientData(acceptSockfd));
            dataToSharedMemory = VectorMapper::serialize(this->data.getBoards());

            char char_array[dataToSharedMemory.length() + 1];
            strcpy(char_array, dataToSharedMemory.c_str());
            sprintf(shared_memory, char_array, sizeof(char_array));

            mtx.unlock();
            shmdt(shared_memory);

            this->Send(acceptSockfd, dataToSend);
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
        wait(nullptr);
        close(acceptSockfd);
    }
    close(sockfd);
//    cout << "dojeli jsme kamo" << endl;

}


sockaddr_in Server::fillStructure() {
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(this->port);

    return (serverAddr);
}

int Server::createSocket() {
    int option = 1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ((sockfd < 0)) {
        perror("socketError:");
        exit(10);
    }
    return sockfd;
}

void Server::bindSocket(int sockfd, sockaddr_in serverAddr) {

    if (bind(sockfd, (SA *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind Error: ");
        exit(10);
    }
}


void Server::Listen(int sockfd) {
    if (listen(sockfd, BACKLOG) < 0) {
        perror("listen eror: ");
        exit(10);
    }
}

int Server::Accept(int sockfd, sockaddr_in clientAddr) {
    socklen_t clientAddrSize = sizeof(clientAddr);

    int acceptedSock = accept(sockfd, (SA *) &clientAddr, &clientAddrSize);

    if (acceptedSock < 0) {
        perror("accept error: ");
        exit(10);
    }
    return (acceptedSock);
}


char *Server::createSharedMemory(int &segment_id) {
    shmctl(segment_id, IPC_RMID, nullptr);
    segment_id = shmget(IPC_PRIVATE, 0x6400,
                        IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    return (char *) shmat(segment_id, nullptr, 0);
}

int *Server::createMutexSharedMemory(int &segment_id, struct shmid_ds &shmbuffer) {
    shmctl(segment_id, IPC_RMID, nullptr);
    segment_id = shmget(IPC_PRIVATE, 0x6400,
                        IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    return (int *) shmat(segment_id, nullptr, 0);
}

vector<string> Server::parseClientData(int clientSock) {
    string recvData;
    vector<string> response;
    recvData = this->Recv(clientSock, 0);

    RequestParser requestParser = RequestParser(recvData);
    response = requestParser.process();

    return (response);
}

void Server::Send(int acceptSockfd, const string &data) {
    char newData[data.size() + 1];
    strcpy(newData, data.c_str());
    if (send(acceptSockfd, newData, strlen(newData), 0) < 0) {
        SOCKET_ERR("Send error: ", SEND_SOCK_ERR)
    }
}


string Server::Recv(int clientSock, int flag) {
    char data[BUFF_SIZE];
    bzero(data, BUFF_SIZE);
    int recData = recv(clientSock, data, BUFF_SIZE, flag);

    if (recData < 0) {
        SOCKET_ERR("Recv", "Recv error occured ")
    } else if (recData == 0) {
        this->endOfData(clientSock);
    }
    string s(data);
    return (s);
}

string Server::processClientData(vector<string> data) {
    return (this->data.process(move(data)));
}

void Server::endOfData(int sockfd) {
    close(sockfd);
//    cout << "dojeli jsme kamo" << endl;
    exit(0);
}

