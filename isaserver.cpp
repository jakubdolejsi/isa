#include "ServerArgumentParser.h"


using namespace std;

int main(int argc, char **argv) {

    ServerArgumentParser serverArgumentParser =  ServerArgumentParser(argc, argv);
    serverArgumentParser.parse();

    RETURN_OK
}