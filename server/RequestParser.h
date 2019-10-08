//
// Created by jakub on 03.10.19.
//

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include "Data.h"
#include <regex>

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

    vector<string>  process() {
        vector<string> response;
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

            exit(99);
            // TODO ERROR!
        }
        return (response);
    }

private:


    vector<string>  processGET() {
        return (this->parseHeader(this->request, false));
    }

    vector<string>  processPOST() {
        return (this->parseHeader(this->request, true));
    }

    vector<string>  processPUT() {
        return (this->parseHeader(this->request, true));
    }

    vector<string>  processDELETE() {
        return (this->parseHeader(this->request, true));
    }


    vector<string> parseHeader(string inRequest, bool content) {
        size_t position = inRequest.find('\n');
        string parsedRequest;
        string lines = inRequest.substr(0, position);
        size_t httpPosition = lines.find("HTTP/1.1");
        vector< string > arr;
        if (httpPosition == -1) {
            //spatna verze http
            exit(20);
        }
        parsedRequest = inRequest.substr(0, httpPosition);
        arr = this->toArray(parsedRequest, arr);
        if (!this->checkIfHostIsPresent(inRequest)) {
            // hhtp/1.1 musi obsahovat host
            exit(65);
        }
        if (content) {
            arr.push_back(this->searchForContent(inRequest));
        }

        return (arr);
    }

    vector<string > toArray(string s, vector <string> arr) {

        replace(s.begin(), s.end(), '/', ' ');
        istringstream iss(s);
        string word;
        int i = 0;
        while (iss >> word) {
            arr.push_back(word);
            i++;
        }
        return (arr);
    }


    unsigned countWords(string str) {
        char char_array[str.length() + 1];
        strcpy(char_array, str.c_str());
        char *arr = char_array;

        int state = 0;
        unsigned wc = 0; // word count
        while (*arr) {
            if (*arr == ' ' || *arr == '\n' || *arr == '\t') {
                state = 0;
            } else if (state == 0) {
                state = 1;
                ++wc;
            }
            ++arr;
        }
        return wc;
    }

    string searchForContent(string inRequest) {
        string line;
        istringstream stream(inRequest);
        bool cLength = false;
        bool cType = false;
        size_t contentLength = 0;
        string content;
        string last;

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
            last = this->last(inRequest);
            if (contentLength == strlen(last.c_str()) - 1) {
                content.append(last);
            }
        }
        return (content);
    }


    string last(string s) {
        int fin = strlen(s.c_str());
        int len = 0;
        string x = "";
        for (int i = fin; i > 0; i--) {
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
            if (line.substr(0, 4) == "Host") {
                found = true;
            }
        }
        return found;
    }


};