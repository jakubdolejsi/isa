#include "server/ServerArgumentParser.h"


using namespace std;

int main(int argc, char **argv) {


    ServerArgumentParser serverArgumentParser =  ServerArgumentParser();
//    serverArgumentParser.parse();


    Server server = serverArgumentParser.createServer();
    server.mainLoop();

    RETURN_OK
}