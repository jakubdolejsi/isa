
#include "server/ServerArgumentParser.h"

int main(int argc, char** argv)
{

    ServerArgumentParser serverArgumentParser = ServerArgumentParser(argc, argv);
    serverArgumentParser.parse();

    Server server = serverArgumentParser.createServer();
    server.mainLoop();
}