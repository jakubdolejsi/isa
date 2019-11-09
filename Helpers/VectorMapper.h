//
// Created by jakub on 14.10.19.
//

#ifndef ISA_VECTORMAPPER_H
#define ISA_VECTORMAPPER_H

#include <sstream>
#include <vector>

using std::istringstream;
using std::string;
using std::vector;

/**
 * @brief Objekt, ktery se stara o serializaci vektoru boards to retezce charu a
 * naopak
 */
class VectorMapper {

public:
    /**
     * @brief Serializuje vektor nastenek do retezce
     * @param boards Nastenka boards
     * @return vraci serializovany vektor
     */
    static string serialize(vector<vector<string>> boards);

    /**
     * @brief Deserializuje data z sharedMemoryData do vektoru boards
     * @param sharedMemoryData data v sdilene promenne
     * @param first testing
     * @return vraci slozeny vektor boards
     */
    static vector<vector<string>> deserialize(char* sharedMemoryData,
            bool& first);
};

#endif // ISA_VECTORMAPPER_H
