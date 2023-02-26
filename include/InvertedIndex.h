#pragma once

#include "ConverterJSON.h"


struct Entry {
    size_t doc_id, count;

// Данный оператор необходим для проведения тестовых сценариев
    bool operator==(const Entry &other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};


class InvertedIndex {
public:
    InvertedIndex() = default;


/**
* Обновить или заполнить базу документов, по которой будем совершать
поиск*
@param texts_input содержимое документов
*/
    void UpdateDocumentBase(std::vector<std::string> input_docs) {
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


/**
* Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
    std::vector<Entry> GetWordCount(const std::string &word) {
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

private:
    ConverterJSON converterJson;
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};