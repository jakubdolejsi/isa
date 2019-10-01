//
// Created by jakub on 01.10.19.
//

#ifndef MAIN_CLIENTARGUMENTPARSER_H
#define MAIN_CLIENTARGUMENTPARSER_H

#endif //MAIN_CLIENTARGUMENTPARSER_H

#include <cstring>
#include "Error.h"

#define COMPARE(X, Y)\
            (strncmp(X, Y, strlen(X)) == 0) ? true : false\

using namespace std;


class ClientArgumentParser {


private:
    string host;
    int port;
    string command;
    int argc;
    char **argv;


public:
    ClientArgumentParser(int argc, char **argv) {
        this->argc = argc;
        this->argv = argv;
    }

    void parse() {
        bool helpOption = false;
        if (this->validateArgumentCount() == 7){
            helpOption = true;
        }
        this->iter();
    }

private:

    int validateArgumentCount() {
        if (this->argc < 6) {
            EXIT(ARGUMENT_COUNT_ERROR)
        }
        return this->argc;
    }

    void iter() {
        for (int i = 1; i < this->argc; ++i) {
            if (strncmp(this->argv[i], "-H", strlen(this->argv[i])) == 0) {
                this->parseHost(this->argv[i + 1], i);
            } else if (strncmp(this->argv[i], "-p", strlen(this->argv[i])) == 0) {
                this->parsePort(this->argv[i + 1], i);
            } else if (strncmp(this->argv[i], "-h", strlen(this->argv[i])) == 0) {
                this->setHelp(); // pujde pryc
            } else {
                this->parseCommand(this->argv, i);
            }
        }
    }

    void parseCommand(char **inCommand, int &i) {
        if (COMPARE("boards", inCommand[i])) {
            this->processBoards(inCommand, i);
        } else if (COMPARE("board", inCommand[i])) {
            this->processBoard(inCommand, i);
        } else if (COMPARE("item", inCommand[i])) {
            this->processItem(inCommand, i);
        } else {
            cout << "Spatny request" << endl;
            exit(432);
        }
    }

    void setHelp() {

    }

    void processBoards(char **inCommand, int &i) {
        i++;
        if (inCommand[i] == NULL) { // boards
            cout << " send request GET/boards " << endl;
        } else if (COMPARE(inCommand[i], "list")) { // boards list
            this->processBoardsList(inCommand, i);
        } else {
            cout << "neznamy prikaz na request" << endl;
            exit(420);
        }
    }

    void processBoardsList(char **inCommand, int &i) {
        i++;
        nullCheck(inCommand[i]);
        if (inCommand[i] == NULL) {
            cout << " exit, nezadane <name>" << endl;
        }
        char *name = inCommand[i];
        cout << " -----------------------------------" << endl;
        cout << " sending request GET/boards/" << name << endl;
        cout << " <name> : "<< name <<endl;
        cout << " ------------------------------------" << endl;
    }

    void processBoard(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        if (COMPARE(inCommand[i], "add")) { // board/add
            this->processBoardAdd(inCommand, i);
        } else if (COMPARE(inCommand[i], "delete")) { //  board/delete
            processBoardDelete(inCommand, i);
        } else {
            cout << "neznamy prikaz na request" << endl;
            exit(420);
        }
    }

    void processBoardDelete(char **inCommand, int &i) {
        i++;
        nullCheck(inCommand[i]);
        if (inCommand[i] == NULL) {
            cout << "exit, nezadane name " << endl;
        }
        char *name = inCommand[i];
        cout << " ------------------------------------" << endl;
        cout << "sending request DELETE /boards/" << name<< endl;
        cout << " <name> : "<< name << endl;
        cout << " ------------------------------------" << endl;
    }

    void processBoardAdd(char **inCommand, int &i) {
        i++;
        nullCheck(inCommand[i]);
        if (inCommand[i] == NULL) {
            cout << " exit, nezadane <name>" << endl;
        }
        char *name = inCommand[i];
        cout << " ------------------------------------" << endl;
        cout << " sending request POST /boards/" << name << endl;
        cout << " <name> : "<< name << endl;
        cout << " ------------------------------------" << endl;
    }

    void processItem(char **inCommand, int &i) {
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


    void processItemAdd(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemAddName(inCommand, i, inCommand[i]);

    }

    void processItemAddName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        char *content = inCommand[i];
        cout << " ------------------------------------" << endl;
        cout << " sending request POST /board/"<< name << endl;
        cout << " <name> : " << name << endl;
        cout << " <content> : " << content<< endl;
        cout << " ------------------------------------" << endl;

    }

    void processItemDelete(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemDeleteName(inCommand, i, inCommand[i]);

    }

    void processItemDeleteName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        char *id = inCommand[i];
        cout << " ------------------------------------" << endl;
        cout << " sending request DELETE /board/" << name<< "/" << id << endl;
        cout << " <name> : " << name << endl;
        cout << " <id> : " << id << endl;
        cout << " ------------------------------------" << endl;
    }

    void processItemUpdate(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemUpdateName(inCommand, i, inCommand[i]);
    }

    void processItemUpdateName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        char *id = inCommand[i];
        this->processItemUpdateName(inCommand, i, name, id);

    }

    void processItemUpdateName(char **inCommand, int &i, char *name, char *id){
        i++;
        this->nullCheck(inCommand[i]);
        char *content = inCommand[i];
        cout << " ------------------------------------" << endl;
        cout << " sending request PUT /board/" << name<< "/" << id << endl;
        cout << " <name> : " << name << endl;
        cout << " <id> : " << id << endl;
        cout << " <content> : " << content << endl;
        cout << " ------------------------------------" << endl;

    }

    void parsePort(char *inPort, int &i) {
        i++;
    }

    void parseHost(char *host, int &i) {
        i++;
    }

    void nullCheck(char *ptr) {
        if (ptr == NULL) { EXIT(60)
        }
    }
};

