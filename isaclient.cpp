//
// Created by jakub on 01.10.19.
//
#include "client/ClientArgumentParser.h"


int main(int argc, char **argv) {


    ClientArgumentParser clientArgumentParser = ClientArgumentParser(argc, argv);
    clientArgumentParser.parse();

    Client client = clientArgumentParser.createClient();
    client.setIpByHost();
    client.connectToServer();

//    DUMP_REQUEST(client)

//    RETURN_OK
}
