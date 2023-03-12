#include "InvertedIndex.h"


    void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {

        for (int i = 0; i < input_docs.size(); ++i)
        {
            std::ifstream openDocs (input_docs[i]);
            if (!openDocs)
            {
                std::cout << "file \"" << input_docs[i] << "\" not found" << std::endl;
                input_docs.erase(input_docs.cbegin() + i);
                i--;
            } else openDocs.close();
        }

       std::string str;

        for (int i = 0; i < input_docs.size(); ++i)
        {
            std::ifstream openDocs(input_docs[i]);
            std::getline(openDocs, str);
            if (str.empty())
            {
                std::cout << "File \"" << input_docs[i] << "\" empty." << std::endl;
                input_docs.erase(input_docs.cbegin() + i);
                i--;
            } else openDocs.close();
        }


        std::ifstream read("config.json");
        nlohmann::json jsonRead;
        read >> jsonRead;
        read.close();


        std::ofstream configFile("config.json");
        nlohmann::json config;

        config["config"]["name"] = jsonRead["config"]["name"];
        config["config"]["version"] = jsonRead["config"]["version"];
        config["config"]["max_responses"] = jsonRead["config"]["max_responses"];
        config["files"] = input_docs;

        configFile << config.dump(2);
        configFile.close();

    }


    std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
        docs = converterJson.GetTextDocuments();

        Entry entry;
        std::vector<Entry> entryVector;
        std::vector<std::string> words;
        std::vector<std::vector<std::string>> result;
        std::string wordResult;
        int count = 0;

        for (int j = 0; j < docs.size(); ++j) {
            for (int i = 0; i <= docs[j].size(); ++i) {
                if (docs[j][i] == ' ' || i == docs[j].size()) {
                    words.push_back(wordResult);
                    wordResult = "";
                } else wordResult += docs[j][i];
            }
            result.push_back(words);
            words.clear();
        }

        for (int j = 0; j < result.size(); ++j) {
            for (int i = 0; i < result[j].size(); ++i) {
                if (result[j][i] == word) {
                    count++;
                }
            }
            if (count != 0) {
                entry.doc_id = j;
                entry.count = count;
                entryVector.push_back(entry);
                count = 0;
            }
        }

        if (entryVector.size() != 0) {
            freq_dictionary.insert(make_pair(word, entryVector));
        }
        return entryVector;
    }

std::vector<std::string> InvertedIndex::GetListFiles()
{
    std::ifstream read("config.json");
    nlohmann::json jsonRead;
    read >> jsonRead;
    read.close();
    return jsonRead["files"];
}
