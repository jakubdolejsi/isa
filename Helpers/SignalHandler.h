//
// Created by jakub on 17.11.19.
//

#ifndef ISA_SIGNALHANDLER_H
#define ISA_SIGNALHANDLER_H

class SignalHandler {

public:

    /**
     * @brief Odstrani sdielne pameti
     */
    static void freeResources();

    /**
     * @brief Ulozi id sdlene pameti pro data
     * @param id id sdlene pameti
     */
    static void setSharedMemoryId(int id);

    /**
     * @brief ulozi id sdilene pameti pro mutex
     * @param id id slidene pameti
     */
    static void setMutexId(int id);

private:
    static int sharedMemoryId;

    static int mutexId;

};

#endif //ISA_SIGNALHANDLER_H
