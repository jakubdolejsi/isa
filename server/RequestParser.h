//
// Created by jakub on 14.10.19.
//

#ifndef ISA_REQUESTPARSER_H
#define ISA_REQUESTPARSER_H

#include <algorithm>
#include <cstring>
#include <sstream>
#include <vector>

using std::istringstream;
using std::string;
using std::vector;

class RequestParser {

private:
    /**
     * @brief request
     */
    string request;

public:
    explicit RequestParser(string request);

    /**
     * @brief Dle metody dotoazu zavola prislusnou metodu na parsovani dotazu
     * @return Vrati vektor s naparsovanymi argumenty metody
     */
    vector<string> process();

private:
    /**
     * @brief Zpracuje GET request
     * @return
     */
    vector<string> processGET();

    /**
     * @brief Zpracuje POST request
     * @return
     */
    vector<string> processPOST();

    /**
     * @brief zpracuje PUT request
     * @return
     */
    vector<string> processPUT();

    /**
     * @brief zpracuje DELETE request
     * @return
     */
    vector<string> processDELETE();

    /**
     * @brief Provede parsing hlavicky
     * @param inRequest vstupni request
     * @param content content requestu
     * @return
     */
    vector<string> parseHeader(const string& inRequest, bool content);

    /**
     * @brief Prevede retezec na vektor
     * @param s vstupni retezec
     * @return slozeny vektor
     */
    vector<string> toArray(string s);

    /**
     * @brief Prohleda v http hlavicce content
     * @param inRequest
     * @return Nalezeny content
     */
    string searchForContent(const string& inRequest);

    /**
     * @brief
     * @param s
     * @return
     */

    string last(string s);

    /**
     * @brief Zkontroluje, jestli http hlavicka obsahuje polozku Host, kterou
     * verze HTTP1.1 obsahovat musi
     * @param inRequest vstupni request
     * @return true pokud se nasla, jinak false
     */
    bool checkIfHostIsPresent(const string& inRequest);
};

#endif // ISA_REQUESTPARSER_H
