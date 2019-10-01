#include "ServerArgumentParser.h"

#define RETURN_OK\
        cout << "vse ok" << endl;\
        return 0;\

using namespace std;

int main(int argc, char **argv) {

    ServerArgumentParser serverArgumentParser =  ServerArgumentParser(argc, argv);
    serverArgumentParser.parse();

    RETURN_OK
}