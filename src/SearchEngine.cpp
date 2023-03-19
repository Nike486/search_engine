#include "SearchEngine.h"

std::mutex mtx;

void createHTML (std::vector<std::vector<std::pair<int, float>>> answers, std::vector<std::basic_string<char>> requests, ConverterJSON converterJSON, std::vector<std::basic_string<char>> listDocResult)
{
    std::ofstream HTMLFile("result.html");

    for (int i = 0; i < answers.size(); ++i) {
        HTMLFile << "<h1> Запрос " << i + 1 << "(" << requests[i] << ")" << ": </h1>" << std::endl;
        for (int j = 0; j < answers[i].size(); ++j) {
            if (answers[i][j].second > 0 && converterJSON.GetResponsesLimit() > j) {
                HTMLFile << "<p>" << j + 1 << ")" << "<a href=\"" << listDocResult[answers[i][j].first] << "\">"
                         << listDocResult[answers[i][j].first] << "</a></p>" << std::endl;
            } else if (answers[i][j].second <= 0)
            {
                HTMLFile << "<p> Не найдено </p>" << std::endl;
            }
        }
        HTMLFile
                << "<p> ======================================================================================================= </p>"
                << std::endl;
    }
}

void recordWords (std::vector<std::vector<std::vector<Entry>>> &VVVEntry, std::vector<std::vector<std::string>> VVString, std::vector<std::vector<Entry>> VEntry, int i, InvertedIndex _index, std::vector<int> &counter)
{
    mtx.lock();
    for (int j = 0; j < VVString[i].size(); ++j) {
        auto r = _index.GetWordCount(VVString[i][j]);
        VEntry.push_back(r);
    }

    VVVEntry.push_back(VEntry);
    VEntry.clear();
    counter.push_back(i);
    mtx.unlock();
}


    std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {

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
        std::vector<std::vector<std::vector<Entry>>> sort;
        std::vector<std::vector<Entry>> VEntry;
        std::vector<std::thread> threads;
        std::vector<int> counter;


        for (int i = 0; i < VVString.size(); ++i) {
            threads.emplace_back(recordWords, std::ref(sort), VVString, VEntry, i, _index, std::ref(counter));
        }

        for (int i = 0; i < threads.size(); ++i) {
            threads[i].join();
        }


        VVVEntry = sort;
        for (int j = 0; j < counter.size(); ++j) {
            for (int i = 0; i < counter.size(); ++i) {
                if (j == counter[i]) {
                    VVVEntry[j] = sort[i];
                    break;
                }
            }
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


    std::vector<std::vector<std::pair<int, float>>> SearchServer::completionAnswers() {
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