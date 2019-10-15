//
// Created by jakub on 14.10.19.
//

#ifndef ISA_DATA_H
#define ISA_DATA_H

#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::to_string;
using std::regex;


class Data {
private:
    /**
     * @brief Vektor vektoru stringu, ktery uchovava informace o nastenkach
     */
    vector<vector<string>> boards;

public:
    /**
     * @brief Zpracuje data requestu
     * @param data Vstupni vektor dat, ktery mu preda object RequestParser
     * @return Vraci zpracovana http data
     */
    string process(vector<string> data);


    /**
     * @brief Nastavi vektor boards dle parametru
     * @param boards vektor boardu
     */
    void setBoards(vector<vector<string>> boards);

    /**
     * @brief Ziska vektor boards
     * @return
     */
    vector<vector<string>> getBoards();

private:


    /**
     * @brief Zpracuje GET request
     * @param params vektor parametru
     * @return Vraci hhtp odpoved dle validity dotazu
     */
    string processGET(vector<string> params);

    /**
     * @brief Zpracuje POST request
     * @param params vektor parametru
     * @return Vraci hhtp odpoved dle validity dotazu
     */
    string processPOST(vector<string> params);

/**
 * @brief Zpracuje PUT request
 * @param params vektor parametru
 * @return Vraci hhtp odpoved dle validity dotazu
 */
    string processPUT(vector<string> params);

    /**
     * @brief Zpracuje DELETE request
     * @param params vektor parametru
     * @return Vraci hhtp odpoved dle validity dotazu
     */
    string processDELETE(vector<string> params);

    /**
     * @brief Vrati http hlavicku OK dotazu
     * @param code Kod http hlavicky validniho dotazu
     * @param data volitelny parametr, pokud budeme chtit krom hlavicky poslat i data
     * @return vrati http odpoved
     */
    string querySucess(const string &code, const string &data = "");

    /**
     * @brief Vrati http hlavicku NOK dotazu
     * @param code Kod hlavicky chybneho dotazu
     * @return vrati http odpoved
     */
    string queryFailed(const string &code);

    /**
     * @brief Overi duplicitu nastenky
     * @param item hledany item
     * @return true pokud je duplicita, false pokud je zaznam unikatni ??
     */
    bool checkDuplicity(const vector<string> &item);

    /**
     * @brief Vytvori novou nastenku
     * @param boardName nazev vytvarene nastenky
     * @return true pokud vytvareni probehlo v poradku, jinak false
     */
    bool createNewBoard(const string &boardName);

/**
 * @brief POST /board/<name>
 * @param boardName
 * @param content
 * @return
 */
    bool insertNewTopic(const string &boardName, string &content);

/**
 * @brief DELETE /boards/<name>
 * @param boardName
 * @return
 */
    bool deleteBoardByName(const string &boardName);


/**
 * @brief Zmeni poradi nasledujicich prvku po akutalnim mazanem
 * @param board
 * @param index
 */
    void reformateTopics(vector<string> &board, int index);

/**
 * @brief DELETE /board/<name>/<id>
 * @param boardName
 * @param id
 * @return
 */
    bool deleteTopicByID(const string &boardName, const string &id);

/**
 * @brief Ziska topiky boardu dle indexu
 * @param j index boardu
 * @return
 */
    string getTopics(int j);

/**
 * @brief GET /board/<name>
 * @param boardName
 * @return Nalezene topiky
 */
    string getBoardByName(const string &boardName);

/**
 * @brief GET /boards
 * @return
 */
    string getAllBoards();

/**
 * @brief Nahradi content aktualniho prispevku za novy
 * @param board
 * @param id
 * @param content
 * @return
 */
    bool update(vector<string> &board, const string &id, string content);

/**
 * @brief PUT /board/<name>/<id>
 * @param boardName
 * @param id
 * @param content
 * @return
 */
    bool updateTopicById(const string &boardName, const string &id, const string &content);

    /**
     * @brief Prida hranate zavorky pred a za nazev boardu
     * @param name
     * @return
     */
    string convertName(string name);

};

#endif //ISA_DATA_H
