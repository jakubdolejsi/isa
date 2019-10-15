//
// Created by jakub on 14.10.19.
//

#ifndef ISA_CLIENTARGUMENTPARSER_H
#define ISA_CLIENTARGUMENTPARSER_H


#include <cstring>
#include "Client.h"
#include "../error/Error.h"

using std::string;
using std::to_string;

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
    const char *content{};
public:
    /**
     * @brief Konstruktor
     * @param argc pocet argumentu
     * @param argv pole argumentu
     */
    ClientArgumentParser(int argc, char **argv);

    /**
     * @brief Spusti parsovani argumentu
     */
    void parse();

    /**
     * @brief Vytvori objekt klienta, kterymu preda request, host a port
     * @return
     */
    Client createClient();

private:

    /**
     * @brief Overeni poctu parametru
     * @return Pocet parametru
     */
    int validateArgumentCount();

    /**
     * @brief Iterace pres pole argumentu
     */
    void iter();

    /**
     * @brief Provede parse http prikazu
     * @param inCommand prikaz, ktery rozhoduje, co se ma provadet dle API
     * @param i pozice v poli argv
     */
    void parseCommand(char **inCommand, int &i);

    /**
     * @brief Nastavi help flag
     */
    void setHelp();

    /**
     * @brief GET /boards
     * @param i pozice v poli argv
     */
    void processBoards(int &i);

    /**
     * @brief GET /board/<name>
     * @param inCommand Pole argumentu
     * @param i pozice v poli argumentu
     */
    void processBoardsList(char **inCommand, int &i);

    /**
     * @brief Zpracuje prikazy tykajici se konkretni nastenky
     * @param inCommand  Pole argumentu
     * @param i pozice v poli argument
     */
    void processBoard(char **inCommand, int &i);

    /**
     * @brief DELETE /boards/<name>
     * @param inCommand  Pole argumentu
     * @param i pozice v poli argumentu
     */
    void processBoardDelete(char **inCommand, int &i);

    /**
     * @brief POST /boards/<name>
     * @param inCommand  Pole argumentu
     * @param i pozice v poli argumentu
     */
    void processBoardAdd(char **inCommand, int &i);

    /**
     * @brief Zpracuje prikazy tykajici se konkretniho itemu nastenky
     * @param inCommand  Pole argumentu
     * @param i pozice v poli argumentu
     */
    void processItem(char **inCommand, int &i);


    /**
     * @brief Zpracuje prikaz pro pridani noveho itemu do nastenky
     * @param inCommand Pole argumentu
     * @param i pozice v poli argumentu
     */
    void processItemAdd(char **inCommand, int &i);

    /**
     * @brief POST /board/<name> [content]
     * @param inCommand pole argumentu
     * @param i pozice v poli argumentu
     * @param name jmeno nastenky
     */
    void processItemAddName(char **inCommand, int &i, char *name);

    /**
     * @brief
     * @param inCommand pole argumentu
     * @param i pozice v poli argumentu
     */
    void processItemDelete(char **inCommand, int &i);

    /**
     * @brief DELETE /board/<name>/<id>
     * @param inCommand
     * @param i pozice v poli argv
     * @param name
     */
    void processItemDeleteName(char **inCommand, int &i, char *name);

    /**
     * @brief
     * @param inCommand
     * @param i pozice v poli argv
     */
    void processItemUpdate(char **inCommand, int &i);

    /**
     * @brief
     * @param inCommand
     * @param i pozice v poli argv
     * @param name
     */
    void processItemUpdateName(char **inCommand, int &i, char *name);


    /**
     * @brief  PUT /board/<name>/<id> [content]
     * @param inCommand
     * @param i pozice v poli argv
     * @param name
     * @param id
     */
    void processItemUpdateName(char **inCommand, int &i, char *name, char *id);

    // TODO
    /**
     * @brief Provede parse portu
     * @param inPort
     * @param i pozice v poli argv
     */
    void parsePort(char **inPort, int &i);


    /**
     * @brief preved port na integer
     * @param inPort
     */
    void portToInteger(const char *inPort);

    /**
     * @brief Overi range portu
     */
    void validatePortRange();


    // TODO
    void parseHost(char **inHost, int &i);

    /**
     * @brief Overi jestli parametr ptr neni null
     * @param ptr testovany parametr
     */
    void nullCheck(const char *ptr);

    /**
     * @brief Overi, jestli je nastaveny port
     */
    void isSetPort();

    /**
     * @brief Over, jestli je nastaveny host
     */
    void isSetHost();
};

#endif //ISA_CLIENTARGUMENTPARSER_H
