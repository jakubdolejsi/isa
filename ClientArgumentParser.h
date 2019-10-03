//
// Created by jakub on 01.10.19.
//

#ifndef MAIN_CLIENTARGUMENTPARSER_H
#define MAIN_CLIENTARGUMENTPARSER_H

#endif //MAIN_CLIENTARGUMENTPARSER_H

#include <cstring>
#include "Error.h"
#include "Client.h"




#define COMPARE(X, Y)\
            (strncmp(X, Y, strlen(X)) == 0) ? true : false\

using namespace std;

/**
 * @brief Trida na parsovani argumentu
 */
class ClientArgumentParser {


private:
    string host = "";
    int port = 0;
    string request{};
    int argc;
    char **argv;
    string content;


public:
    /**
     * @brief Konstruktor
     * @param argc
     * @param argv
     */
    ClientArgumentParser(int argc, char **argv) {
        this->argc = argc;
        this->argv = argv;
    }

    /**
     * @brief Metoda, kterou se spusi parsovani
     */
    void parse() {
        bool helpOption = false;
        if (this->validateArgumentCount() == 7) {
            helpOption = true;
        }
        this->iter();
        this->isSetPort();
        this->isSetHost();
    }

    Client createClient()
    {
        return Client(this->request, this->host, this->port, this->content);
    }



private:

    /**
     * @brief Overeni parametru
     * @return
     */
    int validateArgumentCount() {
        if (this->argc < 6) {
            EXIT(ARGUMENT_COUNT_ERROR)
        }
        return this->argc;
    }

    /**
     * @brief Iterace pres pole argumentu
     */
    void iter() {
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

    /**
     * @brief
     * @param inCommand
     * @param i
     */
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

    /**
     * @brief  GET /boards
     * @param inCommand
     * @param i
     */
    void processBoards(char **inCommand, int &i) {
        i++;
        if (inCommand[i] == nullptr) { // boards
            this->request = "GET/boards ";
//            cout << " send request "<< this->request << endl;
        } else if (COMPARE(inCommand[i], "list")) { // boards list
            this->processBoardsList(inCommand, i);
        } else {
            cout << "neznamy prikaz na request" << endl;
            exit(420);
        }
    }

    /**
     * @brief GET /board/<name>
     * @param inCommand
     * @param i
     */
    void processBoardsList(char **inCommand, int &i) {
        i++;
        nullCheck(inCommand[i]);
        char *name = inCommand[i];
        this->request = string("GET/boards/").append(name);

//        cout << " -----------------------------------" << endl;
//        cout << " sending request "<< this->request << endl;
//        cout << " <name> : " << name << endl;
//        cout << " ------------------------------------" << endl;
    }

    /**
     * @brief
     * @param inCommand
     * @param i
     */
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

    /**
     * @brief DELETE /boards/<name>
     * @param inCommand
     * @param i
     */
    void processBoardDelete(char **inCommand, int &i) {
        i++;
        nullCheck(inCommand[i]);
        char *name = inCommand[i];
        this->request = string("DELETE /boards/").append(name);
//        cout << " ------------------------------------" << endl;
//        cout << "sending request " << this->request << endl;
//        cout << " <name> : " << name << endl;
//        cout << " ------------------------------------" << endl;
    }

    /**
     * @brief POST /boards/<name>
     * @param inCommand
     * @param i
     */
    void processBoardAdd(char **inCommand, int &i) {
        i++;
        nullCheck(inCommand[i]);
        char *name = inCommand[i];
        this->request = string("POST /boards/").append(name);
//        cout << " ------------------------------------" << endl;
//        cout << " sending request " << this->request << endl;
//        cout << " <name> : " << name << endl;
//        cout << " ------------------------------------" << endl;
    }

    /**
     * @brief
     * @param inCommand
     * @param i
     */
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


    /**
     * @brief
     * @param inCommand
     * @param i
     */
    void processItemAdd(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemAddName(inCommand, i, inCommand[i]);

    }

    /**
     * @brief POST /board/<name>
     * @param inCommand
     * @param i
     * @param name
     */
    void processItemAddName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        this->content = inCommand[i];
        this->request = string("POST /board/").append(name);
//        cout << " ------------------------------------" << endl;
//        cout << " sending request " << this->request << endl;
//        cout << " <name> : " << name << endl;
//        cout << " <content> : " << content << endl;
//        cout << " ------------------------------------" << endl;

    }

    /**
     * @brief
     * @param inCommand
     * @param i
     */
    void processItemDelete(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemDeleteName(inCommand, i, inCommand[i]);

    }

    /**
     * @brief DELETE /board/<name>/<id>
     * @param inCommand
     * @param i
     * @param name
     */
    void processItemDeleteName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        char *id = inCommand[i];
        this->request = string("DELETE /board/").append(name).append("/").append(id); // snad povali
        //TODO check na int ID
//        cout << " ------------------------------------" << endl;
//        cout << " sending request " << this->request << endl;
//        cout << " <name> : " << name << endl;
//        cout << " <id> : " << id << endl;
//        cout << " ------------------------------------" << endl;
    }

    /**
     * @brief
     * @param inCommand
     * @param i
     */
    void processItemUpdate(char **inCommand, int &i) {
        i++;
        this->nullCheck(inCommand[i]);
        this->processItemUpdateName(inCommand, i, inCommand[i]);
    }

    /**
     * @brief
     * @param inCommand
     * @param i
     * @param name
     */
    void processItemUpdateName(char **inCommand, int &i, char *name) {
        i++;
        this->nullCheck(inCommand[i]);
        // TODO CHECK NA INT id
        this->processItemUpdateName(inCommand, i, name, inCommand[i]);

    }


    /**
     * @brief  PUT /board/<name>/<id>
     * @param inCommand
     * @param i
     * @param name
     * @param id
     */
    void processItemUpdateName(char **inCommand, int &i, char *name, char *id) {
        i++;
        this->nullCheck(inCommand[i]);
        char *content = inCommand[i];
        this->request = string("PUT /board/").append(name).append("/").append(id).append("/").append(content);
//        cout << " ------------------------------------" << endl;
//        cout << " sending request " << this->request << endl;
//        cout << " <name> : " << name << endl;
//        cout << " <id> : " << id << endl;
//        cout << " <content> : " << content << endl;
//        cout << " ------------------------------------" << endl;

    }

    // TODO
    /**
     * @brief
     * @param inPort
     * @param i
     */
    void parsePort(char **inPort, int &i) {
        i++;
        if (inPort[i] == nullptr) {
            EXIT(WRONG_FORMAT)
        }
        portToInteger(inPort[i]);
        validatePortRange();
    }


    /**
     * @brief
     * @param inPort
     */
    void portToInteger(const char *inPort) {
        string ports(inPort);
        for (char i : ports) {
            if (isdigit(i) == 0) {
                EXIT(UNKNOWN_PORT_FORMAT)
            }
        }
        this->port = stoi(ports);
    }

    /**
     * @brief
     */
    void validatePortRange() {
        if (this->port <= 0 || this->port > 65535) {
            EXIT(WRONG_PORT_RANGE)
        }
    }


    // TODO
    void parseHost(char **inHost, int &i) {
        i++;
        this->nullCheck(inHost[i]);
        this->host = inHost[i];
    }

    void nullCheck(const char *ptr) {
        if (ptr == nullptr) {
            EXIT(60)
        }
    }

    void isSetPort() {
        if(this->port == 0) {
            EXIT(UNKNOWN_PORT_FORMAT) // port neni nastaveny
        }
    }

    void isSetHost() {
        if(this->host == "") {
            EXIT(UNKNOWN_OPTION); // bude jina chyba
        }
    }
};

