//
// Created by jakub on 01.10.19.
//

#include "ServerArgumentParser.h"

ServerArgumentParser::ServerArgumentParser(int argc, char** argv)
{
    this->argc = argc;
    this->argv = argv;
}

void ServerArgumentParser::parse()
{
    this->isSetHelp();
    if (!this->argvArrayIter()) { EXIT(UNKNOWN_OPTION)
    }
    this->validatePortRange();
}

bool ServerArgumentParser::isSetHelp()
{
    for (int i = 1; i<this->argc; ++i) {
        if (strncmp(this->argv[i], "-h", strlen(this->argv[i]))==0) {
            if (argc==2) {
                PRINT_HELP
            }
            else {
                EXIT(ARGUMENT_COUNT_ERROR)
            }
        }
    }
}

Server ServerArgumentParser::createServer()
{
    return (Server(this->port));
}

bool ServerArgumentParser::argvArrayIter()
{
    bool foundP = false;
    char* option = "-p";
    for (int i = 0; i<this->argc; ++i) {
        int len = this->compareLength(this->argv[i], option);
        if (len>0) {
            if (strncmp(this->argv[i], option, len)==0) {
                this->validatePort(this->argv[i+1]);
                foundP = true;
                break;
            }
        }
    }
    return foundP;
}

int ServerArgumentParser::compareLength(char* optionP, char* option)
{
    if (strlen(optionP)==strlen(option)) {
        return strlen(option);
    }
    else {
        return 0;
    }
}

void ServerArgumentParser::validatePort(char* inPort)
{
    if (inPort==nullptr) {
        EXIT(WRONG_FORMAT)
    }
    string ports(inPort);
    for (char i : ports) {
        if (isdigit(i)==0) {
            EXIT(UNKNOWN_PORT_FORMAT)
        }
    }
    this->port = stoi(ports);
}

void ServerArgumentParser::validatePortRange()
{
    if (this->port<=0 || this->port>65535) {
        EXIT(WRONG_PORT_RANGE)
    }
}

int ServerArgumentParser::validateArgumentCount()
{
    if (this->argc==3) {
        return 3;
    }
    else if (this->argc==4) {
        return 4;
    }
    else {
        EXIT(ARGUMENT_COUNT_ERROR)
    }
}
