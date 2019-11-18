//
// Created by jakub on 03.10.19.
//

#include "RequestParser.h"

#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"

#define COMPARE(X, Y) (strncmp(X, Y, strlen(X)) == 0) ? true : false

RequestParser::RequestParser(string request)
{
    this->request = move(request);
}

vector<string> RequestParser::process()
{
    vector<string> response;
    char* method;
    char buf[this->request.size()+1];
    strcpy(buf, this->request.c_str());

    method = strtok(buf, " ");
    if (COMPARE(method, GET)) {
        response = processGET();
    }
    else if (COMPARE(method, POST)) {
        response = this->processPOST();
    }
    else if (COMPARE(method, PUT)) {
        response = this->processPUT();
    }
    else if (COMPARE(method, DELETE)) {
        response = this->processDELETE();
    }
    else {
        response = this->processGET(); // default
    }
    return (response);
}

vector<string> RequestParser::processGET()
{
    return (this->parseHeader(this->request, false));
}

vector<string> RequestParser::processPOST()
{
    return (this->parseHeader(this->request, true));
}

vector<string> RequestParser::processPUT()
{
    return (this->parseHeader(this->request, true));
}

vector<string> RequestParser::processDELETE()
{
    return (this->parseHeader(this->request, false));
}

vector<string> RequestParser::parseHeader(const string& inRequest,
        bool content)
{
    vector<string> arr;
    string parsedRequest;
    size_t newLinePosition = inRequest.find('\n');
    size_t httpPosition = inRequest.substr(0, newLinePosition).find("HTTP/1.1");
    if (httpPosition==-1) {
        // spatna verze http
        exit(20);
    }
    parsedRequest = inRequest.substr(0, httpPosition);
    arr = this->toArray(parsedRequest);
    if (!this->checkIfHostIsPresent(inRequest)) {
        // hhtp/1.1 musi obsahovat host
        exit(65);
    }
    if (content) {
        arr.push_back(this->searchForContent(inRequest));
    }
    return (arr);
}

vector<string> RequestParser::toArray(string s)
{
    vector<string> arr;
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

string RequestParser::searchForContent(const string& inRequest)
{
    string line;
    istringstream stream(inRequest);
    bool cLength = false;
    bool cType = false;
    size_t contentLength = 0;
    string content;
    string last;
    this->request = inRequest;

    while (getline(stream, line)) {
        if (line.substr(0, 16)=="Content-Length: ") {
            cLength = true;
            contentLength = stoi(line.substr(16));
        }
        if (line.substr(0, 14)=="Content-Type: ") {
            cType = true;
        }
    }

    if (cLength && cType) {
        last = this->last(inRequest);
        if (!last.empty() && contentLength==0) {
            return content;
        }
        if (contentLength==strlen(last.c_str())-1) {
            content.append(last);
        }
    }
    return (content);
}

string RequestParser::last(string s)
{
    int fin = strlen(s.c_str());
    int len = 0;
    string x;
    for (int i = fin; i>0; i--) {
        if (s[i]=='\n' && s[i-1]=='\r' && s[i-2]=='\n' &&
                s[i-3]=='\r') {
            len = i;
            break;
        }
    }
    return (s.substr(len));
}

bool RequestParser::checkIfHostIsPresent(const string& inRequest)
{
    string line;
    istringstream stream(inRequest);
    bool found = false;

    while (getline(stream, line)) {
        if (line.substr(0, 4)=="Host") {
            found = true;
        }
    }
    return found;
}
