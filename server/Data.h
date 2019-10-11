//
// Created by jakub on 07.10.19.
//

#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include "VectorMapper.h"

#ifndef ISA_DATA_H
#define ISA_DATA_H

#define DUPLICATE "409"
#define NO_DATA "404"
#define ZERO_LENGTH "400"
#define POST_OK "201"
#define G_P_D_OK "200"

#define COMPARE(X, Y)\
            (strncmp(X, Y, strlen(X)) == 0) ? true : false\


using namespace std;

class Data {

private:
    vector<vector<string>> boards;
    VectorMapper vectorMapper;

public:
    string process(vector<string> data) {
        string result;
        string method = data[0];
        if (method == "GET") {
            result = this->processGET(data);
        } else if (method == "POST") {
            result = this->processPOST(data);
        } else if (method == "DELETE") {
            result = this->processDELETE(data);
        } else if (method == "PUT") {
            result = this->processPUT(data);
        }
        return (result);
    }

    string serialize() {
        string out;
        for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            for (vector<int>::size_type j = 0; j != this->boards[i].size(); j++) {
                out.append(this->boards[i][j]).append("\n");
            }
            out.append(":\n");
        }
        return out;
    }

    void deserialize(char *in, bool &first) {
        first = false;
        vector<vector<string>> boards;
        vector<string> topic;
        istringstream iss(in);
        string line;

        if(first) {
            this->boards = boards;
            first = false;
        }

        while (std::getline(iss, line)) {
            if (line == ":") {
                boards.push_back(topic);
                topic.clear();
            } else {
                topic.push_back(line);
            }
        }
        if(!topic.empty()) {
            boards.push_back(topic);
            topic.clear();
        }
        this->boards = boards;
    }

    vector<vector<string>> getBoards() {
        return this->boards;
    }

private:

    string toString(char **in) {
        string bar;
        for(int i =0 ; i< strlen(*in);i++)
        {
            for(int j =0 ;j<3;j++)
            {
                bar += in[i][j];
            }
        }
        return bar;
    }


    string processGET(vector<string> params) {
        string res;

        if (params[1] == "boards") {
            res = this->getAllBoards();
            if (res.empty()) {
                return this->queryFailed(NO_DATA);
            }
        } else {
            res = this->getBoardByName(this->convertName(params[2]));
            if (res.empty()) {
                return this->queryFailed(NO_DATA);
            }
        }
        return querySucess(G_P_D_OK, res);
    }

    string processPOST(vector<string> params) {
        string firstParam = params[1]; // boards || board
        string name = params[2]; // <name>
        string res;

        if (firstParam == "boards") { // vtvori novou prazdnou nastenku s nazvem <name>
            res = this->createNewBoard(this->convertName(name));
            if (res.empty()) {
                return this->queryFailed(DUPLICATE);
            }
        } else {
            res = this->insertNewTopic(this->convertName(name), params[3]);
            if(res.empty()) {
                return this->queryFailed(NO_DATA);
            }
        }
        return this->querySucess(POST_OK);
    }


    string processPUT(vector<string> params) {
        bool res;
        res = this->updateTopicById(this->convertName(params[2]), params[3], params[4]);

        return (res ? this->querySucess(G_P_D_OK) : this->queryFailed(NO_DATA));
    }

    string processDELETE(vector<string> params) {
        bool res;
        string boardName = params[2];
        if(params[1] == "board") {
            string id = params[3];
            res = this->deleteTopicByID(this->convertName(boardName), id);
        } else {
            res = this->deleteBoardByName(this->convertName(boardName));
        }
        return (res ? this->querySucess(G_P_D_OK) : this->queryFailed(NO_DATA));
    }

    string querySucess(const string &code, string data = "") {
        string header = string("HTTP/1.1 ").append(code).append(" OK\r\n\r\n").append(data);
        return header;
    }
    string queryFailed(const string &code) {
        string header = string("HTTP/1.1 ").append(code).append(" NOK\r\n\r\n");
        return header;
    }

    /**
 * @brief
 * @param vec
 * @param item
 */
    bool checkDuplicity(vector<string> item) {
        return !(std::find(this->boards.begin(), this->boards.end(), item) != this->boards.end());
    }

/**
 * @brief POST /boards/<name>
 * @param boards
 * @param name
 * @return
 */
    string createNewBoard(string boardName) {

        vector<string> cont;
        cont.push_back(boardName);
        bool ok = checkDuplicity(cont);
        if (!ok) {
            return "";
        }
        this->boards.push_back(cont);

        return "OK";
    }

/**
 * @brief POST /board/<name>
 * @param boards
 * @param boardName
 * @param content
 * @return
 */
    string insertNewTopic(const string &boardName, string &content) {

        content.erase(std::remove(content.begin(), content.end(), '\n'), content.end());
        vector<string> cont;
        cont.push_back(boardName);
        if(this->checkDuplicity(cont)) {
            return "";
        }
        static int counter;
        string x;
        for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            if (this->boards[i][0] == boardName) {
                counter = boards[i].size();
                this->boards[i].push_back((to_string(counter)).append(". ").append(content)); // vlozi prispevek
                counter++;
            }
        }
        return "OK";
    }

/**
 * @brief DELETE /boards/<name>
 * @param boards
 * @param boardName
 */
    bool deleteBoardByName(const string &boardName) {
        bool found = false;
        for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            if (this->boards[i][0] == boardName) {
                found = true;
                this->boards.erase(this->boards.begin() + i);
//                this->boards[i].clear();
//                this->boards[i].erase(this->boards[i].begin(), this->boards[i].end());
            }
        }
        return found;
    }

    void deleteTopics(vector<string> &board) {
        for (vector<int>::size_type i = 0; i != boards.size(); i++) {
            board[i].erase(board[i].begin() + i);
        }
    }

/**
 * @brief Zmeni poradi nasledujicich prvku po akutalnim mazanem
 * @param board
 * @param index
 */
    void reformateTopics(vector<string> &board, int index) {
        string oldTopicNumber;
        string newTopicNumber = to_string(index).append(". ");
        for (vector<int>::size_type i = index; i != board.size(); i++) {
            oldTopicNumber = board[i].substr(0, 3);
            board[i] = std::regex_replace(board[i], std::regex(oldTopicNumber), newTopicNumber);
        }
    }

/**
 * @brief DELETE /board/<name>/<id>
 * @param boards
 * @param boardName
 * @param id
 */
    bool deleteTopicByID(const string &boardName, const string &id) {
        int index = atoi(id.c_str());
        bool found = false;
        for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            if (this->boards[i][0] == boardName) {
                found = true;
                this->boards[i].erase(this->boards[i].begin() + index);
                reformateTopics(this->boards[i], index);
            }
        }
        return found;
    }

/**
 * @brief Ziska topiky boardu
 * @param boards
 */
    string getTopics(int j) {
        string res;
        for (vector<int>::size_type i = 0; i != this->boards[j].size(); i++) {
            res.append(this->boards[j][i]).append("\n");
        }
        return res;
    }

/**
 * @brief GET /board/<name>
 * @param boards
 * @param boardName
 * @return
 */
    string getBoardByName(string boardName) {
        for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            if (this->boards[i][0] == boardName) {
                return getTopics(i);
//            return boards[i][0];
            }
        }
        return "";
    }

/**
 * @brief GET /boards
 * @param boards
 * @return
 */
    string getAllBoards() {
        string res;
        for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            res.append(this->boards[i][0]).append("\n");
        }
        return res;
    }

/**
 * @brief Nahradi content aktualniho prispevku za novy
 * @param board
 * @param id
 * @param content
 * @return
 */
    string update(vector<string> &board, string id, string content) {
        size_t pos;
        for (vector<int>::size_type i = 0; i != board.size(); i++) {
            pos = board[i].find('.');
            if (board[i].substr(0, pos) == id) { // pokud je cislo pred teckou shodne s zadanym ID
                content.erase(std::remove(content.begin(), content.end(), '\n'), content.end()); // odstraneni eol
                pos += 2; // nastaveni position na mezeru na ideckem a teckou
                board[i].replace(pos, string::npos, content);
                return board[i];
            }
        }
    }

/**
 * @brief PUT /board/<name>/<id>
 * @param boards
 * @param boardName
 * @param id
 * @param content
 * @return
 */
    bool updateTopicById(string boardName, string id, string content) {
        bool found = false;
        for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            if (this->boards[i][0] == boardName) {
                found = true;
                update(this->boards[i], id, content);
            }
        }
        return found;
    }

    string convertName(string name) {
        return name.insert(0, "[").append("]");
    }

    void print(vector<vector<string>> const &input) {
        for (int i = 0; i < input.size(); i++) {
            for (int j = 0; j < input.size(); j++) {
                cout << input[i][j] << endl;
            }
        }
    }

};

#endif //ISA_DATA_H