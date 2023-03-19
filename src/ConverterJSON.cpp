#include "ConverterJSON.h"


auto getJson (std::string nameJson) {
    std::ifstream read(nameJson);
    nlohmann::json jsonRead;
    read >> jsonRead;
    read.close();
    return jsonRead;
}


    std::vector<std::string> ConverterJSON::GetTextDocuments() {
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

    int ConverterJSON::GetResponsesLimit() {
        return getJson("config.json")["config"]["max_responses"];
    }


    std::vector<std::string> ConverterJSON::GetRequests() {
        return getJson("requests.json")["requests"];
    }


    void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
        std::ofstream answersFile("answers.json");
        nlohmann::json jsonAnswers;

        for (int i = 0; i < answers.size(); ++i) {
            for (int j = 0; j < answers[i].size(); ++j) {

                if (answers[i].size() == 1 && answers[i][j].second != 0) {
                    std::pair<std::pair<std::string, int>, std::pair<std::string, float>> pair{{"docid", answers[i][j].first},
                                                                                               {"rank",  answers[i][j].second}};
                    jsonAnswers["answers"]["request" + std::to_string(i + 1)] = pair;
                    jsonAnswers["answers"]["request" + std::to_string(i + 1)]["result"] = true;
                } else if (answers[i].size() > 1 && answers[i][j].second != 0) {
                    std::vector<std::pair<std::pair<std::string, int>, std::pair<std::string, float>>> vectorPair;

                    for (int k = 0; k < answers[i].size(); ++k) {
                        std::pair<std::pair<std::string, int>, std::pair<std::string, float>> pair{{"docid", answers[i][k].first},
                                                                                                   {"rank",  answers[i][k].second}};
                        if (vectorPair.size() < GetResponsesLimit()) {
                            vectorPair.push_back(pair);
                        }

                    }
                    jsonAnswers["answers"]["request" + std::to_string(i + 1)]["relevance"] = vectorPair;
                    jsonAnswers["answers"]["request" + std::to_string(i + 1)]["result"] = true;
                } else jsonAnswers["answers"]["request" + std::to_string(i + 1)]["result"] = false;
            }
        }
        answersFile << jsonAnswers.dump(2);
    }