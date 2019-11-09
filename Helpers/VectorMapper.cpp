//
// Created by jakub on 08.10.19.
//
#include "VectorMapper.h"

string VectorMapper::serialize(vector<vector<string>> boards)
{
    string out;
    for (vector<int>::size_type i = 0; i!=boards.size(); i++) {
        for (vector<int>::size_type j = 0; j!=boards[i].size(); j++) {
            out.append(boards[i][j]).append("\n");
        }
        out.append(":\n");
    }
    return out;
}

vector<vector<string>> VectorMapper::deserialize(char* sharedMemoryData,
        bool& first)
{
    first = false;
    vector<vector<string>> boards;
    vector<string> topic;
    istringstream iss(sharedMemoryData);
    string line;

    if (first) {
        boards = boards;
        first = false;
    }
    while (getline(iss, line)) {
        if (line==":") {
            boards.push_back(topic);
            topic.clear();
        }
        else {
            topic.push_back(line);
        }
    }
    if (!topic.empty()) {
        boards.push_back(topic);
        topic.clear();
    }
    return boards;
}
