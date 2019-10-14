//
// Created by jakub on 14.10.19.
//

#ifndef ISA_SERVERARGUMENTPARSER_H
#define ISA_SERVERARGUMENTPARSER_H

#include "../error/Error.h"
#include "Server.h"

class ServerArgumentParser {

private:
    int port{};
    char **argv;
    int argc;

public:
    /**
     * @brief Kontruktor
     * @param argc pocet argumentu
     * @param argv pole argumentu
     */
    ServerArgumentParser(int argc, char **argv);

    /**
     * @brief Provede hlavni parsovani argumentu
     */
    void parse();

    /**
     * @brief Zkotnrluje flag -h
     * @return
     */
    bool checkForHelpOption();

    /**
     * @brief Vytvori server na portu ulozenym v this->port
     * @return Server object
     */
    Server createServer();

private:

    /**
     * @brief Prochazi pole argumentu a hleda option -p
     * @return
     */
    bool argvArrayIter();

    /**
     * @brief Nejaky divny porovnani
     * @param optionP
     * @param option
     * @return
     */
    int compareLength(char *optionP, char *option);

    /**
     * @brief Overi, jestli je nastaveny port za flagem -p
     * @param inPort
     */
    void validatePort(char *inPort);

    /**
     * @brief  Overi rozsah portu
     */
    void validatePortRange();

    /**
     * @brief Overi pocet argumentu
     * @return pocet argumentu
     */
    int validateArgumentCount();

};

#endif //ISA_SERVERARGUMENTPARSER_H
