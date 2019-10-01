//
// Created by jakub on 01.10.19.
//

#ifndef MAIN_ERROR_H
#define MAIN_ERROR_H

#endif //MAIN_ERROR_H

#define EXIT(X);\
    switch(X){\
        case(10):\
            fprintf(stderr,"Chybny pocet parametru\n");\
            exit(X);\
        default:\
            fprintf(stderr,"Jina chyba");\
            exit(X);\
};


#define ARGUMENT_ERROR 10