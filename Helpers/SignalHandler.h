//
// Created by jakub on 17.11.19.
//

#ifndef ISA_SIGNALHANDLER_H
#define ISA_SIGNALHANDLER_H

class SignalHandler {

public:

    static void freeResources();

    static void setSharedMemoryId(int id);

    static void setMutexId(int id);

private:
    static int sharedMemoryId;

    static int mutexId;

};

#endif //ISA_SIGNALHANDLER_H
