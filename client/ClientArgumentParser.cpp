//
// Created by jakub on 01.10.19.
//



#include "../error/Error.h"
#include "ClientArgumentParser.h"


#define COMPARE(X, Y)\
            (strncmp(X, Y, strlen(X)) == 0) ? true : false\



ClientArgumentParser::ClientArgumentParser(int argc, char **argv) {
        this->argc = argc;
        this->argv = argv;
    }

void ClientArgumentParser::parse() {
        bool helpOption = false;
        if (this->validateArgumentCount() == 7) {
            helpOption = true;
        }
        this->iter();
        this->isSetPort();
        this->isSetHost();
    }

Client ClientArgumentParser::createClient() {
    return Client(this->request, this->host, this->port);
    }


int ClientArgumentParser::validateArgumentCount() {
        if (this->argc < 6) { EXIT(ARGUMENT_COUNT_ERROR)
        }
        return this->argc;
    }


void ClientArgumentParser::iter() {
        for (int i = 1; i < this->argc; ++i) {
            if (strncmp(this->argv[i], "-H", strlen(this->argv[i])) == 0) {
                this->parseHost(this->argv, i); //TODO  mozno i + 1 !!
            } else if (strncmp(this->argv[i], "-p", strlen(this->argv[i])) == 0) {
                this->parsePort(this->argv, i);
            } else if (strncmp(this->argv[i], "-h", strlen(this->argv[i])) == 0) {
                this->setHelp(); // pujde pryc
            } else {
                this->parseCommand(this->argv, i);
            }
        }
    }


void ClientArgumentParser::parseCommand(char **inCommand, int &i) {
        if (COMPARE("boards", inCommand[i])) {
            this->processBoards(inCommand, i);
        } else if (COMPARE("board", inCommand[i])) {
            this->processBoard(inCommand, i);
        } else if (COMPARE("item", inCommand[i])) {
            this->processItem(inCommand, i);
        } else {
            exit(432);
        }
    }

void ClientArgumentParser::setHelp() {

    }

void ClientArgumentParser::processBoards(int &i) {
        i++;
        this->request = string("GET /boards HTTP/1.1\r\nHost: ").append(this->host).append("\r\n\r\n");
    }


void ClientArgumentParser::processBoardsList(char **inCommand, int &i) {
        i++;
    this->nullCheck(inCommand[i]);
        char *name = inCommand[i];
        this->request = string("GET /board/").append(name).append(" HTTP/1.1\r\nHost: ").append(this->host).append(
                "\r\n\r\n");

    }


void ClientArgumentParser::processBoard(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        if (COMPARE(inCommand[i], "add")) { // board/add
            this->processBoardAdd(inCommand, i);
        } else if (COMPARE(inCommand[i], "delete")) { //  board/delete
            this->processBoardDelete(inCommand, i);
        } else if (COMPARE(inCommand[i], "list")) {
            this->processBoardsList(inCommand, i);
        } else {
            exit(420);
        }
    }


void ClientArgumentParser::processBoardDelete(char **inCommand, int &i) {
        i++;
        nullCheck(inCommand[i]);
        char *name = inCommand[i];
        this->request = string("DELETE /boards/").append(name).append(" HTTP/1.1\r\nHost: ").append(this->host).append(
                "\r\n\r\n");

    }


void ClientArgumentParser::processBoardAdd(char **inCommand, int &i) {
        i++;
        nullCheck(inCommand[i]);
        char *name = inCommand[i];
        this->request = string("POST /boards/").append(name).append(" HTTP/1.1\r\nHost: ").append(this->host).append(
                "\r\n\r\n");

    }


void ClientArgumentParser::processItem(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        if (COMPARE(inCommand[i], "add")) {
            this->processItemAdd(inCommand, i);
        } else if (COMPARE(inCommand[i], "delete")) {
            this->processItemDelete(inCommand, i);
        } else if (COMPARE(inCommand[i], "update")) {
            this->processItemUpdate(inCommand, i);
        } else { EXIT(50)
        }
    }


void ClientArgumentParser::processItemAdd(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemAddName(inCommand, i, inCommand[i]);

    }


void ClientArgumentParser::processItemAddName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        this->content = inCommand[i];

        string cont = string("Content-Type: text/plain\r\nContent-Length: ").append(
                to_string(strlen(this->content))).append("\r\n\r\n").append(this->content);
        this->request = string("POST /board/").append(name).append(" HTTP/1.1\r\nHost: ").append(this->host).append(
                "\r\n").append(cont);


    }


void ClientArgumentParser::processItemDelete(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemDeleteName(inCommand, i, inCommand[i]);

    }


void ClientArgumentParser::processItemDeleteName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        char *id = inCommand[i];
        this->request = string("DELETE /board/").append(name).append("/").append(id).append(
                " HTTP/1.1\r\nHost: ").append(this->host).append("\r\n\r\n");
        //TODO check na int ID

    }

void ClientArgumentParser::processItemUpdate(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemUpdateName(inCommand, i, inCommand[i]);
    }


void ClientArgumentParser::processItemUpdateName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        // TODO CHECK NA INT id
        this->processItemUpdateName(inCommand, i, name, inCommand[i]);

    }


void ClientArgumentParser::processItemUpdateName(char **inCommand, int &i, char *name, char *id) {
        i++;
        this->nullCheck(inCommand[i]);
        this->content = inCommand[i];
        string cont = string("Content-Type: text/plain\r\nContent-Length: ").append(
                to_string(strlen(this->content))).append("\r\n\r\n").append(this->content);
        this->request = string("PUT /board/").append(name).append("/").append(id).append("/ HTTP/1.1\r\nHost: ").append(
                this->host).append("\r\n").append(cont);


    }

    // TODO

void ClientArgumentParser::parsePort(char **inPort, int &i) {
        i++;
        if (inPort[i] == nullptr) { EXIT(WRONG_FORMAT)
        }
        portToInteger(inPort[i]);
        validatePortRange();
    }


void ClientArgumentParser::portToInteger(const char *inPort) {
        string ports(inPort);
        for (char i : ports) {
            if (isdigit(i) == 0) { EXIT(UNKNOWN_PORT_FORMAT)
            }
        }
        this->port = stoi(ports);
    }


void ClientArgumentParser::validatePortRange() {
        if (this->port <= 0 || this->port > 65535) { EXIT(WRONG_PORT_RANGE)
        }
    }


    // TODO
    void ClientArgumentParser::parseHost(char **inHost, int &i) {
        i++;
        this->nullCheck(inHost[i]);
        this->host = inHost[i];
    }

void ClientArgumentParser::nullCheck(const char *ptr) {
        if (ptr == nullptr) { EXIT(60)
        }
    }

void ClientArgumentParser::isSetPort() {
        if (this->port == 0) { EXIT(UNKNOWN_PORT_FORMAT) // port neni nastaveny
        }
    }

void ClientArgumentParser::isSetHost() {
        if (this->host.empty()) { EXIT(UNKNOWN_OPTION) // bude jina chyba
        }
    }
