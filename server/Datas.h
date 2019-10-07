//
// Created by jakub on 07.10.19.
//

#include <string>
#include <vector>
#include <iostream>

#ifndef ISA_DATA_H
#define ISA_DATA_H

#define COMPARE(X, Y)\
            (strncmp(X, Y, strlen(X)) == 0) ? true : false\


using namespace std;

class Datas {

private:
    vector<vector<string>> boards;
    string concreteBoard;

public:
    string process(string data) {
        string result;
        if(data.substr(0,3) == "GET") {
            result = this->processGET(data.substr(4));
        } else if (data.substr(0,4) == "POST") {
            this->processPOST();
        } else if (data.substr(0,6) == "DELETE") {
            this->processDELETE();
        } else if (data.substr(0,3) == "PUT") {
            this->processPUT();
        }
        return (result);
    }

private:

    string processGET(string params) {
        cout << params << endl;
        return (params);
    }
    string processPOST() {
    }
    string processPUT() {
    }
    string processDELETE() {
    }
};
#endif //ISA_DATA_H