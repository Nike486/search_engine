#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "gtest/gtest.h"
#include <thread>
#include <mutex>


TEST(sample_test_case, sample_test)
{
EXPECT_EQ(1, 1);

}


auto getJson (std::string nameJson) {
    std::ifstream read(nameJson);
    nlohmann::json jsonRead;
    read >> jsonRead;
    read.close();
    return jsonRead;
}


class ConverterJSON {
public:
    ConverterJSON() = default;


    void setRequests(std::vector<std::string> requests) {
        std::ofstream requestsFile("requests.json");
        nlohmann::json requestsJson;

        requestsJson["requests"] = requests;

        requestsFile << requestsJson.dump(2);
    }


/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    std::vector<std::string> GetTextDocuments() {
        std::vector<std::string> linkText = getJson("config.json")["files"];
        std::vector<std::string> getText;
        std::string recordText;

        for (int i = 0; i < linkText.size(); ++i) {
            std::ifstream file(linkText[i]);
            while (getline(file, recordText)) {
                getText.push_back(recordText);
            }
            file.close();
        }

        return getText;
    }


/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
    int GetResponsesLimit() {
        return getJson("config.json")["config"]["max_responses"];
    }


/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests() {
        return getJson("requests.json")["requests"];
    }


/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
        std::ofstream answersFile("answers.json");
        nlohmann::json jsonAnswers;

        for (int i = 0; i < answers.size(); ++i) {
            for (int j = 0; j < answers[i].size(); ++j) {

                if (answers[i].size() == 1 && answers[i][j].second != 0) {
                    std::pair<std::pair<std::string, int>, std::pair<std::string, float>> pair{{"docid", answers[i][j].first},
                                                                                               {"rank",  answers[i][j].second}};
                    jsonAnswers["answers"]["request00" + std::to_string(i + 1)] = pair;
                    jsonAnswers["answers"]["request00" + std::to_string(i + 1)]["result"] = true;
                } else if (answers[i].size() > 1 && answers[i][j].second != 0) {
                    std::vector<std::pair<std::pair<std::string, int>, std::pair<std::string, float>>> vectorPair;

                    for (int k = 0; k < answers[i].size(); ++k) {
                        std::pair<std::pair<std::string, int>, std::pair<std::string, float>> pair{{"docid", answers[i][k].first},
                                                                                                   {"rank",  answers[i][k].second}};
                        if (vectorPair.size() < GetResponsesLimit()) {
                            vectorPair.push_back(pair);
                        }

                    }
                    jsonAnswers["answers"]["request00" + std::to_string(i + 1)]["relevance"] = vectorPair;
                    jsonAnswers["answers"]["request00" + std::to_string(i + 1)]["result"] = true;
                } else jsonAnswers["answers"]["request00" + std::to_string(i + 1)]["result"] = false;
            }
        }
        answersFile << jsonAnswers.dump(2);
    }
};


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

void recordWords (std::vector<std::vector<std::vector<Entry>>> &VVVEntry, std::vector<std::vector<std::string>> VVString, std::vector<std::vector<Entry>> VEntry, int i, InvertedIndex _index)
{
    std::mutex mtx;
    for (int j = 0; j < VVString[i].size(); ++j)
    {
        mtx.lock();
        auto r = _index.GetWordCount(VVString[i][j]);
        VEntry.push_back(r);
        mtx.unlock();
    }
    mtx.lock();
    VVVEntry.push_back(VEntry);
    VEntry.clear();
    mtx.unlock();
}


struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer {
public:

/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex &idx) : _index(idx) {};

/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input) {

        std::vector<std::vector<std::string>> VVString;
        std::vector<std::string> VString;
        std::string resultWord;

        for (int j = 0; j < queries_input.size(); ++j) {
            for (int i = 0; i <= queries_input[j].size(); ++i) {
                if (queries_input[j][i] == ' ' || i == queries_input[j].size()) {
                    VString.push_back(resultWord);
                    resultWord = "";
                } else resultWord += queries_input[j][i];
            }
            VVString.push_back(VString);
            VString.clear();
        }

        std::vector<std::vector<std::vector<Entry>>> VVVEntry;
        std::vector<std::vector<Entry>> VEntry;
        std::vector<std::thread> threads;

        for (int i = 0; i < VVString.size(); ++i) {
            threads.emplace_back(recordWords, std::ref(VVVEntry), VVString, VEntry, i, _index);
            threads[i].join();
        }

        int sum = 0;
        int max = 0;
        std::vector<std::vector<Entry>> VVResultEntry;
        std::vector<Entry> VResultEntry;
        Entry resultEntry;

        for (int i = 0; i < VVVEntry.size(); ++i) {
            for (int m = 0; m < converterJson.GetTextDocuments().size(); ++m) {
                for (int j = 0; j < VVVEntry[i].size(); ++j) {
                    for (int k = 0; k < VVVEntry[i][j].size(); ++k) {
                        if (VVVEntry[i][j][k].doc_id == m) {
                            sum += VVVEntry[i][j][k].count;
                        }
                    }
                }
                if (sum != 0) {
                    if (sum > max) {
                        max = sum;
                    }
                    resultEntry.doc_id = m;
                    resultEntry.count = sum;
                    VResultEntry.push_back(resultEntry);
                    sum = 0;
                }
            }
            VVResultEntry.push_back(VResultEntry);
            VResultEntry.clear();
        }

        std::vector<std::vector<RelativeIndex>> VVRelativeIndex;
        std::vector<RelativeIndex> VRelativeIndex;
        RelativeIndex relativeIndex;

        for (int i = 0; i < VVResultEntry.size(); ++i) {
            for (int j = 0; j < VVResultEntry[i].size(); ++j) {
                relativeIndex.doc_id = VVResultEntry[i][j].doc_id;
                relativeIndex.rank = float(VVResultEntry[i][j].count) / float(max);
                VRelativeIndex.push_back(relativeIndex);
            }
            VVRelativeIndex.push_back(VRelativeIndex);
            VRelativeIndex.clear();
        }

        return VVRelativeIndex;
    };


    std::vector<std::vector<std::pair<int, float>>> completionAnswers() {
        auto VVRelativeIndex = search(converterJson.GetRequests());
        std::vector<std::vector<std::pair<int, float>>> VVAnswers;
        std::vector<std::pair<int, float>> VAnswers;
        std::pair<int, float> answers;

        for (int i = 0; i < VVRelativeIndex.size(); ++i) {
            for (int j = 0; j < VVRelativeIndex[i].size(); ++j) {
                answers = {VVRelativeIndex[i][j].doc_id, VVRelativeIndex[i][j].rank};
                VAnswers.push_back(answers);
            }
            if (VVRelativeIndex[i].empty()) {
                answers = {0, 0};
                VAnswers.push_back(answers);
            }
            VVAnswers.push_back(VAnswers);
            VAnswers.clear();
        }

        for (int i = 0; i < VVAnswers.size(); ++i) {
            for (int k = 0; k < VVAnswers[i].size(); ++k) {
                for (int j = 1; j < VVAnswers[i].size(); ++j) {
                    if (VVAnswers[i][j].second > VVAnswers[i][j - 1].second) {
                        std::swap(VVAnswers[i][j], VVAnswers[i][j - 1]);
                    }
                }
            }
        }

        return VVAnswers;
    }

private:
    ConverterJSON converterJson;
    InvertedIndex _index;
};


int main() {

    ConverterJSON converterJSON;
    InvertedIndex invertedIndex;
    SearchServer searchServer(invertedIndex);

    converterJSON.setRequests(std::vector<std::string>{"moscow is the capital of russia", "moscow", "on"});


    invertedIndex.UpdateDocumentBase(std::vector<std::string>{"london is the capital of great britain",
                                                              "paris is the capital of france",
                                                              "berlin is the capital of germany",
                                                              "rome is the capital of italy",
                                                              "madrid is the capital of spain",
                                                              "lisboa is the capital of portugal",
                                                              "bern is the capital of switzerland",
                                                              "moscow is the capital of russia",
                                                              "kiev is the capital of ukraine",
                                                              "minsk is the capital of belarus",
                                                              "astana is the capital of kazakhstan",
                                                              "beijing is the capital of china",
                                                              "tokyo is the capital of japan",
                                                              "bangkok is the capital of thailand",
                                                              "welcome to moscow the capital of russia the third rome",
                                                              "amsterdam is the capital of netherlands",
                                                              "helsinki is the capital of finland",
                                                              "oslo is the capital of norway",
                                                              "stockholm is the capital of sweden",
                                                              "riga is the capital of latvia",
                                                              "tallinn is the capital of estonia",
                                                              "warsaw is the capital of poland"});

    converterJSON.putAnswers(searchServer.completionAnswers());

    return 0;
}