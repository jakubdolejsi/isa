//
// Created by jakub on 17.11.19.
//

#include <sys/shm.h>
#include "SignalHandler.h"

int SignalHandler::sharedMemoryId = 0;

int SignalHandler::mutexId = 0;

void SignalHandler::freeResources()
{
    shmctl(SignalHandler::sharedMemoryId, IPC_RMID, nullptr);
    shmctl(SignalHandler::mutexId, IPC_RMID, nullptr);
}
void SignalHandler::setSharedMemoryId(int id)
{
    SignalHandler::sharedMemoryId = id;
}
void SignalHandler::setMutexId(int id)
{
    SignalHandler::mutexId = id;
}

