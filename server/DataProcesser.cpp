//
// Created by jakub on 07.10.19.
//

#include <iostream>
#include "DataProcesser.h"
#include "../Helpers/VectorMapper.h"

string DataProcesser::process(vector<string> data)
{
    string result;
    string method = data[0];
    if (method=="GET") {
        result = this->processGET(data);
    }
    else if (method=="POST") {
        result = this->processPOST(data);
    }
    else if (method=="DELETE") {
        result = this->processDELETE(data);
    }
    else if (method=="PUT") {
        result = this->processPUT(data);
    }
    return (result);
}

void DataProcesser::setBoards(vector<vector<string>> boards)
{
    this->boards = move(boards);
}

vector<vector<string>> DataProcesser::getBoards()
{
    return this->boards;
}

string DataProcesser::processGET(vector<string> params)
{
    string res;
    if (this->queryParameterParser(params, 1)) {
        return this->queryFailed(BAD_REQUEST);
    }
    if (params[1]=="boards") {
        if (!this->queryParameterParser(params, 2)) {
            return this->queryFailed(BAD_REQUEST);
        }
        res = this->getAllBoards();
    }
    else if (params[1]=="board") {
        if (!this->queryParameterParser(params, 3)) {
            return this->queryFailed(BAD_REQUEST);
        }
        res = this->getBoardByName(this->convertName(params[2]));
    }
    else {
        return this->queryFailed(BAD_REQUEST);
    }
    return (!res.empty() ? this->querySucess(G_P_D_OK, res) : this->queryFailed(BAD_REQUEST));
}

string DataProcesser::processPOST(vector<string> params)
{
    if (!this->queryParameterParser(params, 4)) {
        return this->queryFailed(BAD_REQUEST);
    }
    string firstParam = params[1]; // boards || board
    string name = params[2];       // <name>
    bool res;

    if (firstParam=="boards") { // vtvori novou prazdnou nastenku s nazvem <name>
        if (!this->checkBoardNameCorrectness(name)) {
            return this->queryFailed(BAD_REQUEST);
        }
        res = this->createNewBoard(this->convertName(name));
        return (res ? this->querySucess(POST_OK) : this->queryFailed(DUPLICATE));
    }
    else if (firstParam=="board") {
        if (params[3].empty()) {
            return this->queryFailed(ZERO_LENGTH);
        }
        res = this->insertNewTopic(this->convertName(name), params[3]);
        return (res ? this->querySucess(POST_OK) : this->queryFailed(BAD_REQUEST));
    }
    else {
        return this->queryFailed(BAD_REQUEST);
    }
}

string DataProcesser::processPUT(vector<string> params)
{
    if (!(params[1]=="board")) {
        return this->queryFailed(BAD_REQUEST);
    }
    if (!this->queryParameterParser(params, 5)) {
        return this->queryFailed(BAD_REQUEST);
    }
    if (params[4].empty()) {
        return this->queryFailed(ZERO_LENGTH);
    }
    bool res;
    res = this->updateTopicById(this->convertName(params[2]), params[3], params[4]);

    return (res ? this->querySucess(G_P_D_OK) : this->queryFailed(BAD_REQUEST));
}

string DataProcesser::processDELETE(vector<string> params)
{
    bool res;
    if (!this->queryParameterParser(params, 4)) {
        return this->queryFailed(BAD_REQUEST);
    }
    string boardName = params[2];
    if (params[1]=="board") {
        string id = params[3];
        res = this->deleteTopicByID(this->convertName(boardName), id);
    }
    else {
        res = this->deleteBoardByName(this->convertName(boardName));
    }
    return (res ? this->querySucess(G_P_D_OK) : this->queryFailed(BAD_REQUEST));
}

string DataProcesser::querySucess(const string& code, const string& data)
{
    string contentMetaData = "\r\n\r\n";
    if (!data.empty()) {
        contentMetaData = string("\r\nContent-Type: text/plain\r\n")
                .append("Content-Length: ")
                .append(to_string(data.length()))
                .append(contentMetaData);
    }
    string header = string("HTTP/1.1 ")
            .append(code)
            .append(" OK")
            .append(contentMetaData)
            .append(data);
    return header;
}

string DataProcesser::queryFailed(const string& code)
{
    string header = string("HTTP/1.1 ").append(code).append(" NOK\r\n\r\n");
    return header;
}

bool DataProcesser::checkDuplicity(const string& item)
{
    for (vector<int>::size_type i = 0; i!=this->boards.size(); i++) {
        if (this->boards[i][0]==item) {
            return false;
        }
    }
    return true;
}

bool DataProcesser::createNewBoard(const string& boardName)
{

    vector<string> cont;
    cont.push_back(boardName);
    if (!this->checkDuplicity(boardName)) {
        return false;
    }
    this->boards.push_back(cont);

    return true;
}

bool DataProcesser::insertNewTopic(const string& boardName, string& content)
{
    content.erase(remove(content.begin(), content.end(), '\n'), content.end());
    vector<string> cont;
    cont.push_back(boardName);

    static int vecSize;
    for (vector<int>::size_type i = 0; i!=this->boards.size(); i++) {
        if (this->boards[i][0]==boardName) {
            vecSize = boards[i].size();
            this->boards[i].push_back((to_string(vecSize)).append(". ").append(content)); // vlozi prispevek
            return true;
        }
    }
    return false;
}

bool DataProcesser::deleteBoardByName(const string& boardName)
{
    for (vector<int>::size_type i = 0; i!=this->boards.size(); i++) {
        if (this->boards[i][0]==boardName) {
            this->boards.erase(this->boards.begin()+i);
            return true;
        }
    }
    return false;
}

void DataProcesser::reformateTopics(vector<string>& board, int index)
{
    string oldTopicNumber;
    string newTopicNumber;
    int topicNumber = index;
    for (vector<int>::size_type i = index; i!=board.size(); i++) {
        oldTopicNumber = board[i].substr(0, 3);
        newTopicNumber = to_string(topicNumber).append(". ");
        board[i] = regex_replace(board[i], regex(oldTopicNumber), newTopicNumber);
        topicNumber++;
    }
}

bool DataProcesser::deleteTopicByID(const string& boardName, const string& id)
{
    int index = atoi(id.c_str());
    bool found = false;
    if (index<=0) {
        return found;
    }
    for (vector<int>::size_type i = 0; i!=this->boards.size(); i++) {
        if (this->boards[i][0]==boardName && this->boards[i].size()>index) {
            found = true;
            this->boards[i].erase(this->boards[i].begin()+index);
            reformateTopics(this->boards[i], index);
        }
    }
    return found;
}

string DataProcesser::getTopics(int j)
{
    string res;
    for (vector<int>::size_type i = 0; i!=this->boards[j].size(); i++) {
        res.append(this->boards[j][i]).append("\n");
    }
    return res;
}

string DataProcesser::getBoardByName(const string& boardName)
{
    for (vector<int>::size_type i = 0; i!=this->boards.size(); i++) {
        if (this->boards[i][0]==boardName) {
            return this->getTopics(i);
        }
    }
    return "";
}

string DataProcesser::getAllBoards()
{
    string res;
    for (vector<int>::size_type i = 0; i!=this->boards.size(); i++) {
        res.append(this->boards[i][0]).append("\n");
    }
    return res;
}

bool DataProcesser::update(vector<string>& board, const string& id,
        string content)
{
    size_t pos;
    for (vector<int>::size_type i = 0; i!=board.size(); i++) {
        pos = board[i].find('.');
        if (board[i].substr(0, pos)==id) { // pokud je cislo pred teckou shodne s zadanym ID
            content.erase(remove(content.begin(), content.end(), '\n'), content.end()); // odstraneni eol
            pos += 2; // nastaveni position na mezeru za ideckem a teckou
            board[i].replace(pos, string::npos, content);
            return true;
        }
    }
    return false;
}

bool DataProcesser::updateTopicById(const string& boardName, const string& id, const string& content)
{
    bool found = false;
    for (vector<int>::size_type i = 0; i!=this->boards.size(); i++) {
        if (this->boards[i][0]==boardName && this->boards[i].size()>=atoi(id.c_str())) {
            return update(this->boards[i], id, content);
        }
    }
    return found;
}

string DataProcesser::convertName(string name)
{
    return name.insert(0, "[").append("]");
}

bool DataProcesser::queryParameterParser(const vector<string>& query, int paramCount)
{
    return (query.size()==paramCount);
}

bool DataProcesser::checkBoardNameCorrectness(string boards)
{
    for (char board : boards)
        if (!isalnum(board)) {
            return false;
        }
    return true;
}




