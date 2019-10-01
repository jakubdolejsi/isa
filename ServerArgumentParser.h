//
// Created by jakub on 01.10.19.
//

#ifndef MAIN_SERVERARGUMENTPARSER_H
#define MAIN_SERVERARGUMENTPARSER_H

#endif //MAIN_SERVERARGUMENTPARSER_H

#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "Error.h"

using namespace std;

class ServerArgumentParser {

private:
    int port;
    char **argv;
    int argc;

public:
    ServerArgumentParser(int argc, char **argv) {
        this->argc = argc;
        this->argv = argv;
    }

    void parse() {
        this->validateArgumentsCount();
        this->validatePortOption();
        this->validateArgumentsArray();
    }

    int getPort()
    {
        return  this->port;
    }

private:
    void validateArgumentsCount() {
        if (this->argc != 3) {
            EXIT(ARGUMENT_ERROR)
        }
    }

    void validateArgumentsArray() {
        this->getIntegerPort();
        this->validatePortRange();

        const char *p(this->argv[1]);

        if (strncmp(p, "-p", strlen(p)) != 0 ) {
            EXIT(ARGUMENT_ERROR)
        }
    }

    void checkForHelOption()
    {
        for (int i = 0; i < argc; ++i) {
            if(strncmp(this->argv[i],"-h",strlen(this->argv[i])) == 0){

            }
        }
    }

    void getIntegerPort() {
        string ports(this->argv[2]);

        for (char i : ports)
            if (isdigit(i) == 0) {
                EXIT(ARGUMENT_ERROR)
            }
        this->port = stoi(ports);
    }

    void validatePortRange() {
        if (this->port <= 0 && this->port >= 65535){
            EXIT(ARGUMENT_ERROR)
        }
    }

    void validatePortOption()
    {
        if(strncmp(this->argv[1], "-p", strlen(this->argv[1])) != 0){
            EXIT(ARGUMENT_ERROR)
        }
    }
};
