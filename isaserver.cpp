
#include "server/ServerArgumentParser.h"
#include <csignal>
#include <cstdlib>
#include "Helpers/SignalHandler.h"

static volatile int keepRunning = 1;

void signalWrapper(int d)
{
    SignalHandler::freeResources();
    exit(0);
}


int main(int argc, char** argv)
{

    signal(SIGINT, signalWrapper);
    while (keepRunning) {
        ServerArgumentParser serverArgumentParser = ServerArgumentParser(argc, argv);
        serverArgumentParser.parse();

        Server server = serverArgumentParser.createServer();
        server.mainLoop();
    }
    return 0;
}