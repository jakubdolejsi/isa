//
// Created by jakub on 01.10.19.
//

#ifndef MAIN_ERROR_H
#define MAIN_ERROR_H

#define SEND_SOCK_ERR "An error occured while sending data\n"

#define CREATE_SOCK_ERR                                                        \
  "An error occured while creating socket, check your internet connection\n"
#define CONNECT_SOCK_ERR                                                       \
  "Check server availability or rightness of specified ports and try again\n"
#define RECV_SOCK_ERR "An error occured when recieving data from server\n"

#define ARGUMENT_COUNT_ERROR "Invalid arguments count"
#define UNKNOWN_PORT_FORMAT "Invalid port format"
#define UNKNOWN_OPTION "Unknow parameter"
#define WRONG_PORT_RANGE "Port has to be in range 1 - 65535"
#define WRONG_FORMAT "Wrong argument format"
#define NO_PORT "Port has not been set"
#define NO_HOST "Host has not been set"

#define SOCKET_ERR(P_TEXT, M_TEXT)                                             \
  perror(P_TEXT);                                                              \
  fprintf(stderr, M_TEXT);                                                     \
  exit(EXIT_FAILURE);

#define EXIT(TEXT)                                                              \
  fprintf(stderr, TEXT);                                                        \
  exit(EXIT_FAILURE);                                                           \

//
//#define EXIT(X)                                                                \
//  ;                                                                            \
//  switch (X) {                                                                 \
//  case (10):                                                                   \
//    fprintf(stderr, "Chybny pocet parametru\n");                               \
//    exit(X);                                                                   \
//  case (20):                                                                   \
//    fprintf(stderr, "Port musi obsahovat pouze cislicen\n");                   \
//    exit(X);                                                                   \
//  case (30):                                                                   \
//    fprintf(stderr, "Zadan spatny prepinac\n");                                \
//    exit(X);                                                                   \
//  case (40):                                                                   \
//    fprintf(stderr, "Spatny rozsah portu\n");                                  \
//    exit(X);                                                                   \
//  case (50):                                                                   \
//    fprintf(stderr, "Spatny format parametru\n");                              \
//    exit(X);                                                                   \
//  default:                                                                     \
//    fprintf(stderr, "Spatny request\n");                                       \
//    exit(X);                                                                   \
//  };


#endif // MAIN_ERROR_H
