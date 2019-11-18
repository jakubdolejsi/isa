//
// Created by jakub on 14.10.19.
//

#ifndef ISA_SERVER_H
#define ISA_SERVER_H

#include "DataProcesser.h"
#include <netinet/in.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define SA struct sockaddr // makro pro pretypovani struktury
#define BACKLOG 20 // Maximalni pocet spojeni
#define BUFF_SIZE 4096 // Velikost bufferu prijimanych dat, vetsi pro skolni projekt nema smysl vytvaret, nehlede na to ze TCP
// pokud neprijme vsechny data, tak si zbytek kernel ulozi a prijmeme je v dalsim volani
#define TEST_DATA                                                              \
  "[PrvniNastenka]\n1. prvniClanek\n2. druhyClanek\n3. TretiClanek\n4. DalsiClanek\n:\n[DruhaNastenka]\n1. prvniClanekDruheNastenky\n2.OpetDalsiClanek\n:\n[TestNastenka]\n:"

class Server {

private:
    /**
     * @brief port port, na kterem bezi server
     */
    int port;
    /**
     * @brief dataProcesser objekt starajici se o zpracovani dat prijatych od
     * klienta
     */
    DataProcesser dataProcesser;

public:

    /**
     * @brief Konstruktor slouzici k inicializaci portu a objektu DataProcesser
     * @param port port predany objetekm ServerArgumentParser
     */
    explicit Server(int port);

    /**
     * @brief Hlavni smycka serveru, ktera zpracovava data a posila odpoved
     */
    void mainLoop();


private:
    /**
     * @brief Naplni strukturu sockaddr_in potrebnymi daty(rodina adres, port...)
     * @return naplnena struktura
     */
    sockaddr_in fillStructure();

    /**
     * @brief Vytvori server socket
     * @return vytvoreny socket
     */
    int createSocket();

    /**
     * @brief provede bind socketu
     * @param sockfd server socket
     * @param serverAddr naplnena struktura
     */
    void bindSocket(int sockfd, sockaddr_in serverAddr);

    /**
     * @brief Nasloucha na vytvorenem socketu
     * @param sockfd server socket
     */
    void Listen(int sockfd);

    /**
     * @brief Nasloucha na portu specifikovanym v strukture sockaddr_in
     * @param sockfd server socket
     * @param clientAddr naplnena struktura
     * @return klientsky socket
     */
    int Accept(int sockfd, sockaddr_in clientAddr);

    /**
     * @brief Vytvori sdilenou pamet pro uchovavani dat nastenky
     * @param segmentId identifikator sdielne pameti(flagy, velikost)
     * @return Vrati char pripojeny k sdilene pameti
     */
    char* createDataSharedMemory(int& segmentId);

    /**
     * @brief Vytvori sdilenou pamet pro mutexy
     * @param mutexId identifikator sdilene pametu
     * @return Vrati mutex union pripojeny na sdilenou paomet
     */
    sem_t* createMutexSharedMemory(int& mutexId);


    /**
     * @brief Provede parsing prijatych dat
     * @param acceptSockfd klientsky socket
     * @return Naparsovana data do vektoru
     */
    vector<string> parseClientData(int acceptSockfd);

    /**
     * @brief Odesle data od serveru ke klientovi
     * @param acceptSockfd klientsky socket
     * @param data data, ktera se maji odeslat
     */
    void Send(int acceptSockfd, const string& data);

    /**
     * @brief Prijme klientska data
     * @param clientSock Klientsky socket
     * @return prijata data
     */
    string Recv(int clientSock);

    /**
     * @brief Provede zpracovani prijatych dat
     * @param data Naparsovana data ve vektoru
     * @return Zpracovana data (odpoved serveru)
     */
    string processClientData(vector<string> data);

};

#endif // ISA_SERVER_H
