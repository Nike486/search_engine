#include "InvertedIndex.h"


    void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
        std::vector<std::string> setFiles;
        for (int i = 0; i < input_docs.size(); ++i) {
            setFiles.push_back("file00" + std::to_string(i) + ".txt");
            std::ofstream doc("file00" + std::to_string(i) + ".txt");
            doc << input_docs[i];
            doc.close();
        }

        std::ofstream configFile("config.json");
        nlohmann::json config;

        config["config"]["name"] = "SkillboxSearchEngine";
        config["config"]["version"] = "0.1";
        config["config"]["max_responses"] = 5;
        config["files"] = setFiles;

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
