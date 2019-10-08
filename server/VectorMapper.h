//
// Created by jakub on 08.10.19.
//

#ifndef ISA_VECTORMAPPER_H
#define ISA_VECTORMAPPER_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class VectorMapper{

private:
    vector<vector<string>> boards;
    string outputData;

public:
    void save() {
        string out;
        for(vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            for (vector<int>::size_type j = 0; j != this->boards[i].size(); i++) {
                cout << boards[i][j] << endl;
            }
        }
    }

    string load() {
    }

    void setBoards(vector<vector<string>> inBoards) {
        this->boards = inBoards;
    }

};

#endif //ISA_VECTORMAPPER_H
