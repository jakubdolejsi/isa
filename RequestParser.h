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
        if (this->request.substr(START_POS, GET_LEN) == "GET") {
            response = this->processGET(this->request.substr(GET_LEN + 1));
        } else if (this->request.substr(START_POS, POST_LEN) == "POST") {
            response = this->processPOST(this->request.substr(POST_LEN + 1));
        } else if (this->request.substr(START_POS, PUT_LEN) == "PUT") {
            response = this->processPUT(this->request.substr(PUT_LEN + 1));
        } else if (this->request.substr(START_POS, DELETE_LEN) == "DELETE") {
            response = this->processDELETE(this->request.substr(DELETE_LEN + 1));
        } else {
            // TODO ERROR!
        }
        return (response);
    }

private:


    string processPUT(string body) {
        string msg = "Here is your response to PUT request:\nDATAAAAA";

        return (msg);
    }

    string processPOST(string body) {
        string msg = "Here is your response to POST request:\nDATAAAAA";

        return (msg);
    }

    string processGET(string body) {
        string msg = "Here is your response to GET request:\nDATAAAAA";

        return (msg);
    }

    string processDELETE(string body) {
        string msg = "Here is your response to DELETE request:\nDATAAAAA";

        return (msg);
    }

    string getRequest() {
        return (this->request);
    }

};