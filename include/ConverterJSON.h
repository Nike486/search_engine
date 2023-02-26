#pragma once

#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"


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