#include "server/ServerArgumentParser.h"


using namespace std;

int main(int argc, char **argv) {

//
//char *test = "aaaa";
//char *x;
//*(&x) = test;
//
//
//
//exit(0);

    ServerArgumentParser serverArgumentParser =  ServerArgumentParser();
//    serverArgumentParser.parse();


    Server server = serverArgumentParser.createServer();
    server.mainLoop();

    RETURN_OK
}