//
// Created by jakub on 14.10.19.
//

#ifndef ISA_DATAPROCESSER_H
#define ISA_DATAPROCESSER_H

#define DUPLICATE "409 CONFLICT"
#define NOT_FOUND "404 NOT FOUND"
#define BAD_REQUEST "400 BAD REQUEST"
#define POST_OK "201 CREATED"
#define G_P_D_OK "200 OK"

#define COMPARE(X, Y) (strncmp(X, Y, strlen(X)) == 0) ? true : false


#include <regex>
#include <string>
#include <vector>

using std::regex;
using std::string;
using std::to_string;
using std::vector;

class DataProcesser {
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
     * Zpracuje neznamou metodu
     * @param params vektor parametru
     * @return Http odpoved
     */
    string processDefault(vector<string> params);

    /**
     * @brief Vrati http hlavicku OK dotazu
     * @param code Kod http hlavicky validniho dotazu
     * @param data volitelny parametr, pokud budeme chtit krom hlavicky poslat i
     * data
     * @return vrati http odpoved
     */
    string querySucess(const string& code, const string& data = "");

    /**
     * @brief Vrati http hlavicku NOK dotazu
     * @param code Kod hlavicky chybneho dotazu
     * @return vrati http odpoved
     */
    string queryFailed(const string& code);

    /**
     * @brief Overi duplicitu nastenky
     * @param item hledany item
     * @return true pokud je duplicita, false pokud je zaznam unikatni ??
     */
    bool checkDuplicity(const string& item);

    /**
     * @brief Vytvori novou nastenku
     * @param boardName nazev vytvarene nastenky
     * @return true pokud vytvareni probehlo v poradku, jinak false
     */
    bool createNewBoard(const string& boardName);

    /**
     * @brief POST /board/<name>
     * @param boardName
     * @param content
     * @return
     */
    bool insertNewTopic(const string& boardName, string& content);

    /**
     * @brief DELETE /boards/<name>
     * @param boardName
     * @return
     */
    bool deleteBoardByName(const string& boardName);

    /**
     * @brief Zmeni poradi nasledujicich prvku po akutalnim mazanem
     * @param board
     * @param index
     */
    void reformateTopics(vector<string>& board, int index);

    /**
     * @brief DELETE /board/<name>/<id>
     * @param boardName
     * @param id
     * @return
     */
    bool deleteTopicByID(const string& boardName, const string& id);

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
    string getBoardByName(const string& boardName);

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
    bool update(vector<string>& board, const string& id, string content);

    /**
     * @brief PUT /board/<name>/<id>
     * @param boardName
     * @param id
     * @param content
     * @return
     */
    bool updateTopicById(const string& boardName, const string& id,
            const string& content);

    /**
     * @brief Prida hranate zavorky pred a za nazev boardu
     * @param name Jmeno nastenky
     * @return Zkonvertovany nazev nastenky
     */
    string convertName(string name);

    /**
     * Zkontroluje, jestli sedi pocet parametru
     * @param query  Pole parametru dotazu
     * @param paramCount  pocet parametru, se kterym se porovnava pole query
     * @return true pokud se rovanji, false pokud ne
     */
    bool queryParameterChecker(const vector<string>& query, int paramCount);

    /**
     * Overi, jestli nastenka obsahuje pouze alfanumericke znaky
     * @param boardName nazev nastenky
     * @return true pokud je nazev spravne, false pokud neni
     */
    bool checkBoardNameCorrectness(string boardName);
};




#endif // ISA_DATAPROCESSER_H
