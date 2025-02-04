//
// Created by jakub on 02.10.19.
//

#include "Client.h"


Client::Client(string request, string host, int port)
{
    this->request = move(request);
    this->host = move(host);
    this->port = port;
}

// ------------------------ DUMP
// METHODS------------------------------------------
string Client::getRequest()
{
    return (this->request);
}

string Client::getHost()
{
    return (this->host);
}

int Client::getPort()
{
    return (this->port);
}
// -----------------------------------------------------------------------------------

void Client::setIpByHost()
{
    hostent* adr;
    char* ip = nullptr;
    char* hostname = const_cast<char*>(this->host.c_str());

    if ((adr = gethostbyname(hostname))==nullptr) {
        exit(40);
    }
    ip = inet_ntoa(*((struct in_addr*) adr->h_addr_list[0]));
    this->ip = ip;
}

void Client::connectToServer()
{
    string data;
    int sockfd = this->createSocket();

    struct sockaddr_in addr = this->fillStruct();

    this->Connect(sockfd, addr);

    char cstr[this->request.size()+1];
    strcpy(cstr, this->request.c_str());

    this->sendRequest(sockfd, cstr);

    data = this->Recv(sockfd);
    cout << data << endl;

    close(sockfd);
}

string Client::Recv(int sockfd)
{
    char buf[BUF_SIZE];
    bzero(buf, BUF_SIZE);
    string data;
    int rc;
    while ((rc = recv(sockfd, buf, BUF_SIZE, 0))>0) {
        if (rc<0) {
            SOCKET_ERR("Reciev err", "An error occured while recieving dataProcesser")
        }
        data += buf;
        memset(buf, 0, BUF_SIZE);

        this->parseResponse(data);
    }
    return (buf);
}

void Client::sendRequest(int sockfd, char* body)
{
    send(sockfd, body, strlen(body), 0);
}

sockaddr_in Client::fillStruct()
{
    struct sockaddr_in serverAddr;

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    inet_aton(this->ip,
            reinterpret_cast<in_addr*>(
                    &serverAddr.sin_addr.s_addr)); // naplneni serverAddr.sin_addr

    return (serverAddr);
}

int Client::createSocket()
{
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0) {
        SOCKET_ERR("Creating socket error", CREATE_SOCK_ERR)
    }
    return (sockfd);
}

void Client::Connect(int sockfd, sockaddr_in serverAddr)
{
    int conn = connect(sockfd, (SA*) &serverAddr, sizeof(serverAddr));
    if (conn<0) {
        SOCKET_ERR("Connection error", CONNECT_SOCK_ERR)
    }
}

void Client::parseResponse(string response)
{
    size_t found = response.find("\r\n\r\n");
    string body = response.substr(found+4); // posuneme se o 4 znaky dopredu, tj.  2x '\r' + 2x '\n'
    string header = response.substr(0, found);

    char char_array[header.length()+1];
    strcpy(char_array, header.c_str());

    fprintf(stderr, "%s", char_array);
    cout << endl;
    cout << body << endl;
}



