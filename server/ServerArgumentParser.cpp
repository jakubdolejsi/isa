//
// Created by jakub on 01.10.19.
//



#include "../error/Error.h"
#include "Server.cpp"

using namespace std;


#define PRINT_HELP\
            cout << "Naaapoveda "<< endl;\


class ServerArgumentParser {

//private:
//    int port{};
//    char **argv;
//    int argc;
//
//public:
//    ServerArgumentParser(int argc, char **argv) {
//        this->argc = argc;
//        this->argv = argv;
//    }
//
//    void parse() {
//        bool helpOption = false;
//        if(this->validateArgumentCount() == 4) {
//            helpOption = this->checkForHelpOption();
//        }
//        if(!this->argvArrayIter()){
//            EXIT(UNKNOWN_OPTION)
//        }
//        this->validatePortRange();
//        if(helpOption)
//            PRINT_HELP
//    }
//
//    bool checkForHelpOption() {
//        if (strncmp(this->argv[1], "-h", strlen(this->argv[1])) == 0) {
//            return true;
//        } else if(strncmp(this->argv[3], "-h", strlen(this->argv[3])) == 0){
//            return true;
//        }
//        EXIT(UNKNOWN_OPTION)
//    }

public:
    Server createServer() {
        return (Server(5001));
    }

//private:
//
//    bool argvArrayIter()
//    {
//        bool foundP = false;
//        char *option = "-p";
//        for (int i = 0; i < this->argc; ++i) {
//            int len = this->compareLength(this->argv[i], option);
//            if(len > 0) {
//                if (strncmp(this->argv[i], option, len) == 0) {
//                    this->validatePort(this->argv[i + 1]);
//                    foundP = true;
//                    break;
//                }
//            }
//        }
//        return foundP;
//    }
//
//    int compareLength(char *optionP, char *option) {
//        if (strlen(optionP) == strlen(option)) {
//            return strlen(option);
//        } else {
//            return 0;
//        }
//    }
//
//    void validatePort(char *inPort){
//        if(inPort == nullptr){
//            EXIT(WRONG_FORMAT)
//        }
//        string ports(inPort);
//        for (char i : ports)
//            if (isdigit(i) == 0) {
//                EXIT(UNKNOWN_PORT_FORMAT)
//            }
//        this->port = stoi(ports);
//    }
//
//    void validatePortRange() {
//        if (this->port <= 0 || this->port > 65535){
//            EXIT(WRONG_PORT_RANGE)
//        }
//    }
//
//    int validateArgumentCount()
//    {
//        if(this->argc == 3) {
//            return 3;
//        } else if (this->argc == 4){
//            return  4;
//        } else {
//            EXIT(ARGUMENT_COUNT_ERROR)
//        }
//    }
};
