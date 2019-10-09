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

string serialize(vector<vector<string>> boards) {
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

vector<vector<string>>  deserialize(void *in) {
    char *charIn = (char *)in;

    istringstream iss(charIn);
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


void* create_shared_memory(size_t size) {
    // Our memory buffer will be readable and writable:
    int protection = PROT_READ | PROT_WRITE;

    // The buffer will be shared (meaning other processes can access it), but
    // anonymous (meaning third-party processes cannot obtain an address for it),
    // so only this process and its children will be able to use it:
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    // The remaining parameters to `mmap()` are not important for this use case,
    // but the manpage for `mmap` explains their purpose.
    return mmap(NULL, size, protection, visibility, -1, 0);
}



int main(int argc, char **argv) {

//
//    char* parent_message = "hello";  // parent process will write this message
//    char* child_message = "goodbye"; // child process will then write this one
//
//    void* shmem = create_shared_memory(128);
//
//    char *x ="";
//    memcpy(shmem, x, sizeof(x));
//
//    int pid = fork();
//
//    while(true) {
//        if (pid == 0) {
//            printf("This should by empty: %s\n", shmem);
//            memcpy(shmem, child_message, sizeof(child_message));
//            printf("Child wrote: %s\n", shmem);
//
//        }
//        waitpid(pid, NULL, 0);
//        printf("Parent read: %s\n", shmem);
//        printf("After 1s, parent read: %s\n", shmem);
//        memcpy(shmem, parent_message, sizeof(parent_message));
//        printf("Parent write: %s\n", shmem);
//
//    }
//
//    exit(0);

    ServerArgumentParser serverArgumentParser =  ServerArgumentParser();
//    serverArgumentParser.parse();


    Server server = serverArgumentParser.createServer();
    server.mainLoop();

    RETURN_OK
}