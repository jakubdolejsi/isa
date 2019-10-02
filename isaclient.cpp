//
// Created by jakub on 01.10.19.
//
#include <iostream>
#include "ClientArgumentParser.h"

using namespace std;


int main(int argc, char **argv) {



    ClientArgumentParser clientArgumentParser = ClientArgumentParser(argc, argv);
    clientArgumentParser.parse();

    Client client = clientArgumentParser.createClient();
    client.getHostByIp();
    client.connect();

    DUMP_REQUEST(client)

    RETURN_OK
}
