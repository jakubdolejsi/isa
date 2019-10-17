//
// Created by jakub on 07.10.19.
//


#include "../Helpers/VectorMapper.h"
#include "DataProcesser.h"

#define DUPLICATE "409"
#define NO_DATA "404"
#define ZERO_LENGTH "400"
#define POST_OK "201"
#define G_P_D_OK "200"

#define COMPARE(X, Y)\
            (strncmp(X, Y, strlen(X)) == 0) ? true : false\



string DataProcesser::process(vector<string> data) {
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


void DataProcesser::setBoards(vector<vector<string>> boards) {
    this->boards = move(boards);
}

vector<vector<string>> DataProcesser::getBoards() {
    return this->boards;
}


string DataProcesser::processGET(vector<string> params) {
    string res;

    if (params[1] == "boards") {
        res = this->getAllBoards();
    } else {
        res = this->getBoardByName(this->convertName(params[2]));
    }
    return (!res.empty() ? this->querySucess(G_P_D_OK, res) : this->queryFailed(NO_DATA));
}


string DataProcesser::processPOST(vector<string> params) {
    string firstParam = params[1]; // boards || board
    string name = params[2]; // <name>
    bool res;

    if (firstParam == "boards") { // vtvori novou prazdnou nastenku s nazvem <name>
        res = this->createNewBoard(this->convertName(name));
        return (res ? this->querySucess(POST_OK) : this->queryFailed(DUPLICATE));

    } else {
        res = this->insertNewTopic(this->convertName(name), params[3]);
        return (res ? this->querySucess(POST_OK) : this->queryFailed(NO_DATA));
    }
}


string DataProcesser::processPUT(vector<string> params) {
    bool res;
    res = this->updateTopicById(this->convertName(params[2]), params[3], params[4]);

    return (res ? this->querySucess(G_P_D_OK) : this->queryFailed(NO_DATA));
}


string DataProcesser::processDELETE(vector<string> params) {
    bool res;
    string boardName = params[2];
    if (params[1] == "board") {
        string id = params[3];
        res = this->deleteTopicByID(this->convertName(boardName), id);
    } else {
        res = this->deleteBoardByName(this->convertName(boardName));
    }
    return (res ? this->querySucess(G_P_D_OK) : this->queryFailed(NO_DATA));
}


string DataProcesser::querySucess(const string &code, const string &data) {
    string contentMetaData = "\r\n\r\n";
    if (!data.empty()) {
        contentMetaData = string("\r\nContent-Type: text/plain\r\n").append("Content-Length: ").append(
                to_string(data.length())).append(contentMetaData);
    }
    string header = string("HTTP/1.1 ").append(code).append(" OK").append(contentMetaData).append(data);
    return header;
}


string DataProcesser::queryFailed(const string &code) {
    string header = string("HTTP/1.1 ").append(code).append(" NOK\r\n\r\n");
    return header;
}


bool DataProcesser::checkDuplicity(const vector<string> &item) {
    return !(find(this->boards.begin(), this->boards.end(), item) != this->boards.end());
}


bool DataProcesser::createNewBoard(const string &boardName) {

    vector<string> cont;
    cont.push_back(boardName);
    bool ok = checkDuplicity(cont);
    if (!ok) {
        return false;
    }
    this->boards.push_back(cont);

    return true;
}


bool DataProcesser::insertNewTopic(const string &boardName, string &content) {

    content.erase(remove(content.begin(), content.end(), '\n'), content.end());
    vector<string> cont;
    cont.push_back(boardName);
    if (!this->checkDuplicity(cont)) {
        return false;
    }
    static int vecSize;
    for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
        if (this->boards[i][0] == boardName) {
            vecSize = boards[i].size();
            this->boards[i].push_back((to_string(vecSize)).append(". ").append(content)); // vlozi prispevek
            return true;
        }
    }
    return false;
}


bool DataProcesser::deleteBoardByName(const string &boardName) {
    for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
        if (this->boards[i][0] == boardName) {
            this->boards.erase(this->boards.begin() + i);
            return true;
//                this->boards[i].clear();
//                this->boards[i].erase(this->boards[i].begin(), this->boards[i].end());
        }
    }
    return false;
}


void DataProcesser::reformateTopics(vector<string> &board, int index) {
    string oldTopicNumber;
    string newTopicNumber = to_string(index).append(". ");
    for (vector<int>::size_type i = index; i != board.size(); i++) {
        oldTopicNumber = board[i].substr(0, 3);
        board[i] = regex_replace(board[i], regex(oldTopicNumber), newTopicNumber);
    }
}


bool DataProcesser::deleteTopicByID(const string &boardName, const string &id) {
    int index = atoi(id.c_str());
    bool found = false;
    if (index <= 0) {
        return found;
    }
    for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
        if (this->boards[i][0] == boardName && this->boards[i].size() > index) {
            found = true;
            this->boards[i].erase(this->boards[i].begin() + index);
            reformateTopics(this->boards[i], index);
        }
    }
    return found;
}

string DataProcesser::getTopics(int j) {
    string res;
    for (vector<int>::size_type i = 0; i != this->boards[j].size(); i++) {
        res.append(this->boards[j][i]).append("\n");
    }
    return res;
}

string DataProcesser::getBoardByName(const string &boardName) {
    for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
        if (this->boards[i][0] == boardName) {
            return this->getTopics(i);
        }
    }
    return "";
}


string DataProcesser::getAllBoards() {
    string res;
    for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
        res.append(this->boards[i][0]).append("\n");
    }
    return res;
}


bool DataProcesser::update(vector<string> &board, const string &id, string content) {
    size_t pos;
    for (vector<int>::size_type i = 0; i != board.size(); i++) {
        pos = board[i].find('.');
        if (board[i].substr(0, pos) == id) { // pokud je cislo pred teckou shodne s zadanym ID
            content.erase(remove(content.begin(), content.end(), '\n'), content.end()); // odstraneni eol
            pos += 2; // nastaveni position na mezeru na ideckem a teckou
            board[i].replace(pos, string::npos, content);
            return true;
        }
    }
    return false;
}

bool DataProcesser::updateTopicById(const string &boardName, const string &id, const string &content) {
    bool found = false;
    for (vector<int>::size_type i = 0; i != this->boards.size(); i++) {
        if (this->boards[i][0] == boardName && this->boards[i].size() >= atoi(id.c_str())) {
            return update(this->boards[i], id, content);
        }
    }
    return found;
}


string DataProcesser::convertName(string name) {
    return name.insert(0, "[").append("]");
}


