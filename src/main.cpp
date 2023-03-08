#include "ConverterJSON.h"
#include "SearchEngine.h"
#include "InvertedIndex.h"

#include <chrono>

int main() {

    ConverterJSON converterJSON;
    InvertedIndex invertedIndex;
    SearchServer searchServer(invertedIndex);

    std::ifstream readConfig("config.json");
    std::ifstream readRequests("listRequests.txt");

    std::vector<std::string> requests;
    std::string requestsText;


    if (!readRequests) {
        std::ofstream createRequests("listRequests.txt");
        std::cout << "File \"listRequests.txt\" has been created." << std::endl;

    } else if (readRequests) {
        while (!readRequests.eof()) {
            std::string buf;
            readRequests >> buf;
            requestsText += buf + " ";
        }

        std::string requestsRead;

        for (int i = 0; i <= requestsText.size(); ++i) {
            if (requestsText[i] == ',' || i == requestsText.size()) {
                requests.push_back(requestsRead);
                requestsRead = "";
            } else {
                requestsRead += requestsText[i];
            }
        }
    }

    std::vector<std::string> createTextFiles;

    if (!readConfig) {
        int quantityFiles;
        std::cout << "Enter quantity text files: ";
        std::cin >> quantityFiles;

        for (int i = 0; i < quantityFiles; ++i) {
            createTextFiles.push_back("");
        }
        invertedIndex.UpdateDocumentBase(createTextFiles);
        readConfig.close();

        std::cout << "Files has been created. Fill in the files and \"listRequests.txt\"";
    } else {
        std::string str;
        std::ifstream l("listRequests.txt");
        std::getline(l, str);

        if (str.empty()) {
            std::cout << "File \"listRequests.txt\" empty. Fill in the file.";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            return 0;
        }
        l.close();


        auto list = invertedIndex.GetListFiles();
        char answer;
        for (int i = 0; i < list.size(); ++i) {
            std::ifstream file("file00" + std::to_string(i) + ".txt");
            if (!file.is_open()) {
                std::cout << "\"file00" << std::to_string(i) << ".txt\" not found. Add this file? (y/n): ";
                std::cin >> answer;
                if (answer == 'y') {
                    std::ofstream txtFile("file00" + std::to_string(i) + ".txt");
                    std::cout << "File " << "\"file00" + std::to_string(i) + ".txt\"" << " added!";
                    return 0;
                } else return 0;
            }
            file.close();
        }

        for (int i = 0; i < list.size(); ++i) {
            std::ifstream file("file00" + std::to_string(i) + ".txt");
            std::string s;
            std::getline(file, s);
            if (s.empty()) {
                std::cout << "File \"file00" << std::to_string(i) << ".txt\" empty. Fill in the file.";
                std::this_thread::sleep_for(std::chrono::seconds(5));
                return 0;
            }

            file.close();
        }

        converterJSON.setRequests(requests);
        converterJSON.putAnswers(searchServer.completionAnswers());

        std::ofstream HTMLFile("result.html");
        auto answers = searchServer.completionAnswers();

        for (int i = 0; i < answers.size(); ++i) {
            HTMLFile << "<h1> Запрос " << i + 1 << "(" << requests[i] << ")" << ": </h1>" << std::endl;
            for (int j = 0; j < answers[i].size(); ++j) {
                if (answers[i][j].second > 0) {
                    HTMLFile << "<p>" << j + 1 << ")" << "<a href=\"file:file00" << answers[i][j].first
                             << ".txt\">file00" << answers[i][j].first << "</a></p>" << std::endl;
                } else HTMLFile << "<p> Не найдено </p>" << std::endl;
            }
            HTMLFile
                    << "<p> ======================================================================================================= </p>"
                    << std::endl;
        }

        std::cout << "Successfully! The result is in file \"result.html\"";
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}