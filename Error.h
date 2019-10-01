//
// Created by jakub on 01.10.19.
//

#ifndef MAIN_ERROR_H
#define MAIN_ERROR_H

#endif //MAIN_ERROR_H


#define RETURN_OK\
        cout << "---------------------------" << endl;\
        cout << "||      vse ok            ||" << endl;\
        cout << "---------------------------" << endl;\
        return 0;\


#define EXIT(X);\
    switch(X){\
        case(10):\
            fprintf(stderr,"Chybny pocet parametru\n");\
            exit(X);\
    case(20):\
            fprintf(stderr,"Port musi obsahovat pouze cislicen\n");\
            exit(X);\
    case(30):\
            fprintf(stderr,"Zadan spatny prepinac\n");\
            exit(X);\
    case(40):\
            fprintf(stderr,"Spatny rozsah portu\n");\
            exit(X);\
    case(50):\
            fprintf(stderr,"Spatny format parametru\n");\
            exit(X);\
        default:\
            fprintf(stderr,"Jina chyba\n");\
            exit(X);\
};


#define ARGUMENT_COUNT_ERROR 10
#define UNKNOWN_PORT_FORMAT 20
#define UNKNOWN_OPTION 30
#define WRONG_PORT_RANGE 40
#define WRONG_FORMAT 50