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
    string serData;

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

    string serialize(vector<vector<string>> boards) {
        string out;
        for (vector<int>::size_type i = 0; i != boards.size(); i++) {
            for (vector<int>::size_type j = 0; j != boards[i].size(); j++) {
//            cout << boards[i][j] << endl;
                out.append(boards[i][j]).append("\n");
            }
            out.append(":\n");
        }
        this->serData = out;
        return out;
    }

    void deserialize(void *in) {
        stringstream strm;
        strm << in;

        istringstream iss(strm.str());
        vector<vector<string>> boards;
        vector<string> topic;
        string line;

        while (std::getline(iss, line)) {
            if (line == ":") {
                boards.push_back(topic);
                topic.clear();
            } else {
                topic.push_back(line);
            }
        }
        this->boards = boards;
    }

    vector<vector<string>> getBoards() {
        return this->boards;
    }

private:


    string processGET(vector<string> params) {
        string res;

        if (params[1] == "boards") {
            res = this->getAllBoards();
            if (res.empty()) {
                return this->GETFailed(NO_DATA);
            }
        } else {
            res = this->getBoardByName(params[2]);
            if (res.empty()) {
                return this->GETFailed(NO_DATA);
            }
        }
        this->print(this->boards);
        return GETSuccess(G_P_D_OK, res);
    }

    string processPOST(vector<string> params) {
        string firstParam = params[1]; // boards || board
        string name = params[2]; // <name>
        string res;

        if (firstParam == "boards") { // vtvori novou prazdnou nastenku s nazvem <name>
            res = this->createNewBoard(this->convertName(name));
            if (res.empty()) {
                return POSTFailed(DUPLICATE);
            }
        } else {
            res = this->insertNewTopic(this->convertName(name), params[3]);
            if(res.empty()) {
                return POSTSuccess(NO_DATA);
            }
            // nevim kde by se mohla stat chyba pri vkladani
        }
        this->print(this->boards);
        return this->POSTSuccess(POST_OK);
    }


    string processPUT(vector<string> params) {
//        cout << params << endl;
//        exit(0);

    }

    string processDELETE(vector<string>) {
    }


    size_t findContentStart(string inString, string find) {
        string x = inString;
        int found = x.find(find);
        if (found != string::npos) {
            return found;
        }
    }

    string processPOSTBoards(string name) {
        vector<string> cont;
        bool duplicate;
        cont.push_back(name);
        duplicate = this->findDuplicate(this->boards, cont);
        if (duplicate) {
            return (this->POSTFailed(DUPLICATE));
        }
        this->boards.push_back(cont);
        return (this->POSTSuccess(POST_OK));
    }

    string processPOSTBoard(vector<string> params) {
        string board = params[2];
        string content = params[3];
        return "";
    }

    bool findDuplicate(vector<vector<string>> vec, vector<string> item) {
        return (std::find(vec.begin(), vec.end(), item) != vec.end());
    }


    string GETFailed(string code) {
        string header = string("HTTP/1.1 ").append(code).append(" NOK\r\n\r\n");
        return header;
    }

    string GETSuccess(string code, string data) {
        string header = string("HTTP/1.1 ").append(code).append(" OK\r\n\r\n").append(data);
        return header;
    }

    string POSTSuccess(string code) {
        string header = string("HTTP/1.1 ").append(code).append(" OK\r\n\r\n");
        return header;
    }

    string POSTFailed(const string &code) {
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
    string createNewBoard(string name) {

        vector<string> cont;
        cont.push_back(name);
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
    string insertNewTopic(const string &boardName, const string &content) {

        vector<string> cont;
        cont.push_back(boardName);
        bool dupl = checkDuplicity(cont);
        if(dupl) {
            return "";
        }
        static int counter = 1;
        string x;
        for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
            if (this->boards[i][0] == boardName) {
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
    void deleteBoardByName(vector<vector<string>> &boards, string boardName) {

        for (vector<int>::size_type i = 0; i != boards.size(); i++) {
            if (boards[i][0] == boardName) {
                boards.erase(boards.begin() + i);
            }
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
    void deleteTopicByID(vector<vector<string>> &boards, string boardName, string id) {
        int index = atoi(id.c_str());
        for (vector<int>::size_type i = 0; i != boards.size(); i++) {
            if (boards[i][0] == boardName) {
                boards[i].erase(boards[i].begin() + index);
                reformateTopics(boards[i], index);
            }
        }
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
        cout << "Nastenka " << boardName << " nebyla nalezena" << endl;
        return "Nenaslo se";
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
//        cout << "get all boads" << endl;

//        cout << res << endl;
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
    string updateTopicById(vector<vector<string>> &boards, string boardName, string id, string content) {
        for (vector<int>::size_type i = 0; i != boards.size(); i++) {
            if (boards[i][0] == boardName) {
                update(boards[i], id, content);
            }
        }
        return "";
    }

    string convertName(string name) {
        return name.insert(0, "[").append("]");
    }

    void print(vector<vector<string>> const &input) {
        for (int i = 0; i < input.size(); i++) {
            for (int j = 0; j < input.size(); j++) {
                cout << input[i][j] << endl;
            }
//            std::cout << input.at(i) << ' ';
        }
    }


};

#endif //ISA_DATA_H