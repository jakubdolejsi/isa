//
// Created by jakub on 03.10.19.
//

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Datas.h"

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
//        cout << "------------------" << endl;
//        cout << method<< endl;
//        cout << "------------------" << endl;
        if (COMPARE(method, GET)) {
            response = this->processGET();
        } else if (COMPARE(method, POST)) {
            response = this->processPOST();
        } else if (COMPARE(method, PUT)) {
            response = this->processPUT();
        } else if (COMPARE(method, DELETE)) {
            response = this->processDELETE();
        } else {
            response = "Unknown method!";
            exit(99);
            // TODO ERROR!
        }
        return (response);
    }

private:


    string processGET() {
        return (this->parseHeader(this->request, false));
    }

    string processPOST() {
        string result = this->parseHeader(this->request, true);
    }

    string processPUT() {
        return (this->parseHeader(this->request, true));
    }

    string processDELETE() {
        string result = this->parseHeader(this->request, true);
    }


    string parseHeader(string inRequest, bool content) {
        size_t position = inRequest.find('\n');
        string parsedRequest;
        string lines = inRequest.substr(0, position);
        size_t httpPosition = lines.find("HTTP/1.1");
        if(httpPosition == -1) {
            //spatna verze http
            exit(20);
        }
        parsedRequest = inRequest.substr(0, httpPosition);
        if(!this->checkIfHostIsPresent(inRequest)) {
            // hhtp/1.1 musi obsahovat host
            exit(65);
        }
        if(content) {
            parsedRequest.append(this->searchForContent(inRequest));
        }

        return (parsedRequest);
    }

    string searchForContent(string inRequest) {
        string line;
        istringstream stream(inRequest);
        bool cLength = false;
        bool cType = false;
        size_t contentLength = 0;
        string content = "Content: ";
        string last;

//        cout << "-------------------" << endl;
//        cout << inRequest << endl;
//        cout << "-------------------" << endl;
        while (getline(stream, line)) {
            if (line.substr(0, 16) == "Content-Length: ") {
                cLength = true;
                contentLength = stoi(line.substr(16));
            }
            if (line.substr(0, 14) == "Content-Type: ") {
                cType = true;
            }
        }
        if (cLength && cType) {
//            content.append(this->getContent(inRequest));
//            cout << "-------------------" << endl;
//            cout << content << endl;
//            cout << "-------------------" << endl;
            last = this->last(inRequest);
            if(contentLength == strlen(last.c_str())-1) {
                content.append(last);

            }
        }
        return (content);
    }

    int getContentLength() {

    }


    string last(string s) {
        int fin = strlen(s.c_str());
        int len = 0;
        string x ="";
        for (int i = fin; i > 0; i--) {
            x = s.c_str()[i];
            if (s[i] == '\n' && s[i - 1] == '\r' && s[i - 2] == '\n' && s[i - 3] == '\r') {
                len = i;
                break;
            }
        }
        return (s.substr(len));
    }

    bool checkIfHostIsPresent(string lines) {
        string line;
        istringstream stream(lines);
        bool found = false;

        while (getline(stream, line)) {
            if(line.substr(0,4) == "Host") {
                found = true;
            }
        }
        return found;
    }



};