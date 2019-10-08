#include "server/ServerArgumentParser.h"


using namespace std;


/**
 * @brief
 * @param vec
 * @param item
 */
void checkDuplicity(vector<vector<string>> vec, vector<string> item) {
    if ( std::find(vec.begin(), vec.end(), item) != vec.end() )
        exit(99);
}

/**
 * @brief POST /boards/<name>
 * @param boards
 * @param name
 * @return
 */
string createNewBoard(vector<vector<string>> &boards, string name) {

    vector<string > cont;
    cont.push_back(name);
    checkDuplicity(boards, cont);
    boards.push_back(cont);

    return "povedlo se";
}

/**
 * @brief POST /board/<name>
 * @param boards
 * @param boardName
 * @param content
 * @return
 */
string insertNewTopic(vector<vector<string>> &boards, string boardName, string content) {

    static int counter = 1;
    string x;
    for(vector<int>::size_type i = 0; i != boards.size(); i++) {
        if (boards[i][0] == boardName) {
            boards[i].push_back((to_string(counter)).append(". ").append(content)); // vlozi prispevek
            counter++;
        }
    }
    return "";
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
string getTopics(vector<string> &board) {
    string res;
    for (vector<int>::size_type i = 0; i != board.size(); i++) {
        res.append(board[i]).append("\n");
    }
    return res;
}
/**
 * @brief GET /board/<name>
 * @param boards
 * @param boardName
 * @return
 */
string getBoardByName(vector<vector<string>> &boards, string boardName) {
    for (vector<int>::size_type i = 0; i != boards.size(); i++) {
        if (boards[i][0] == boardName) {
            return getTopics(boards[i]);
//            return boards[i][0];
        }
    }
    cout << "Nastenka "<< boardName <<" nebyla nalezena" << endl;
    return "Nenaslo se";
}
/**
 * @brief GET /boards
 * @param boards
 * @return
 */
string getAllBoards(vector<vector<string>> &boards) {
    string res;
    for (vector<int>::size_type i = 0; i != boards.size(); i++) {
        res.append(boards[i][0]).append("\n");
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
    for(vector<int>::size_type i = 0; i != board.size(); i++) {
        pos = board[i].find('.');
        if(board[i].substr(0, pos) == id) { // pokud je cislo pred teckou shodne s zadanym ID
            pos+=2; // nastaveni position na mezeru na ideckem a teckou
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

string ser(vector<vector<string>> boards) {
    string out;
    for (vector<int>::size_type i = 0; i != boards.size(); i++) {
        for (vector<int>::size_type j = 0; j != boards[i].size(); j++) {
//            cout << boards[i][j] << endl;
            out.append(boards[i][j]).append("\n");
        }
        out.append(":\n");
    }
    return out;
}

vector<vector<string>>  deser(string in) {
    istringstream iss(in);
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
    return boards;
}

void print(vector<vector<string>> boards) {
    for (vector<int>::size_type i = 0; i != boards.size(); i++) {
        for (vector<int>::size_type j = 0; j != boards[i].size(); j++) {
            cout << boards[i][j] << endl;
        }
    }
}
int main(int argc, char **argv) {


//    vector<vector<string>> boards;
//    createNewBoard(boards, "[nastenka1]");
//    createNewBoard(boards, "[nastenka2]");
//
//    insertNewTopic(boards, "[nastenka2]", "prvni clanek");
//    insertNewTopic(boards, "[nastenka2]", "druhy clanek");
//    createNewBoard(boards, "[nastenka3]");
//    insertNewTopic(boards, "[nastenka2]", "treti clanek");
//
//    updateTopicById(boards, "[nastenka2]", "2", "to je to kurva druhy clanek!!");
//    cout << "-------------------SER---------------" << endl;
//    string out = ser(boards);
//    cout << out << endl;
//    cout << "--------------------DESER--------------" << endl;
//    vector<vector<string>> newBoards;
//    newBoards = deser(out);
//    print(newBoards);
//
//
//
//
//    exit(0);

    ServerArgumentParser serverArgumentParser =  ServerArgumentParser(argc, argv);
    serverArgumentParser.parse();


    Server server = serverArgumentParser.createServer();
    server.mainLoop();

    RETURN_OK
}