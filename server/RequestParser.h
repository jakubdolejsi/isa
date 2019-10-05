//
// Created by jakub on 03.10.19.
//

#include <cstdio>
#include <cstdlib>
#include <string>

#ifndef ISA_REQUESTPARSER_H
#define ISA_REQUESTPARSER_H
#endif //ISA_REQUESTPARSER_H

#define GET_LEN 3
#define POST_LEN 4
#define PUT_LEN 3
#define DELETE_LEN 6
#define START_POS 0
#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"

#define COMPARE(X, Y)\
            (strncmp(X, Y, strlen(X)) == 0) ? true : false\


using namespace std;

class RequestParser {

private:
    string request;

public:
    RequestParser(string request) {
        this->request = request;
    }

    string process() {
        string response;
        char *method;
        char buf[this->request.size() + 1];
        strcpy(buf, this->request.c_str());

        method = strtok(buf, " ");
        cout << "------------------" << endl;
        cout << method<< endl;
        cout << "------------------" << endl;
        if (COMPARE(method, GET)) {
            response = this->processGET(this->request.substr(GET_LEN + 1));
        } else if (COMPARE(method, POST)) {
            response = this->processPOST(this->request.substr(POST_LEN + 1));
        } else if (COMPARE(method, PUT)) {
            response = this->processPUT(this->request.substr(PUT_LEN + 1));
        } else if (COMPARE(method, DELETE)) {
            response = this->processDELETE(this->request.substr(DELETE_LEN + 1));
        } else {
            response = "chyba kamo vole";
            // TODO ERROR!
        }

        return (response);
    }

private:


    string processPUT(string body) {
        string dataLen = "30";
        string msg = string("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ").append(dataLen);
        msg.append("\n\nZde budou data(odpoved na PUT)");
        return (msg);
    }

    string processPOST(string body) {
        string dataLen = "30";
        string msg = string("HTTP/1.1 201 OK\nContent-Type: text/plain\nContent-Length: ").append(dataLen);
        msg.append("\n\nZde budou data(odpoved na POST)");

        return (msg);
    }

    string processGET(string body) {
        string dataLen = "30";
        string msg = string("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ").append(dataLen);
        msg.append("\n\nZde budou data(odpoved na GET)");
        return (msg);
    }

    string processDELETE(string body) {
        string dataLen = "30";
        string msg = string("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ").append(dataLen);
        msg.append("\n\nZde budou data(odpoved na DELETE)");
        return (msg);
    }

    string getRequest() {
        return (this->request);
    }

};