//
// Created by jakub on 01.10.19.
//
#include <iostream>
#include "ClientArgumentParser.h"

using namespace std;


int main(int argc, char **argv){


    ClientArgumentParser clientArgumentParser = ClientArgumentParser(argc, argv);
    clientArgumentParser.parse();


    RETURN_OK
}
