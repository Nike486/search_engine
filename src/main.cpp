#include "ConverterJSON.h"
#include "SearchEngine.h"
#include "InvertedIndex.h"

#include <chrono>

int main() {

    ConverterJSON converterJSON;
    InvertedIndex invertedIndex;
    SearchServer searchServer(invertedIndex);

    std::ifstream configJson ("config.json");
    std::ifstream requestsJson ("requests.json");

    if (!configJson)
    {
        std::cout << "File \"config.json\" not found" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 0;
    }

    if (!requestsJson)
    {
        std::cout << "File \"requests.json\" not found" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 0;
    }

    try
    {
        auto requests = converterJSON.GetRequests();
        auto listDoc = invertedIndex.GetListFiles();

        invertedIndex.UpdateDocumentBase(listDoc);
        auto listDocResult = invertedIndex.GetListFiles();

        if (listDocResult.size() == 0) {
            std::cout << "Error! Files are missing" << std::endl;

            std::ofstream HTMLFile("result.html");
            HTMLFile << "<p> Error! Files are missing </p>" << std::endl;
            HTMLFile.close();

            std::this_thread::sleep_for(std::chrono::seconds(3));
            return 0;
        }

        if (requests.size() == 0) {
            std::cout << "Error! Requests are missing" << std::endl;

            std::ofstream HTMLFile("result.html");
            HTMLFile << "<p> Error! Requests are missing </p>" << std::endl;
            HTMLFile.close();

            std::this_thread::sleep_for(std::chrono::seconds(3));
            return 0;
        }

        auto answers = searchServer.completionAnswers();

        converterJSON.putAnswers(answers);

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


        std::ifstream read("config.json");
        nlohmann::json jsonRead;
        read >> jsonRead;
        read.close();

        std::cout << "======================================" << std::endl << jsonRead["config"]["name"] << std::endl << "======================================" << std::endl;

        std::cout << "Successfully! The result is in file \"result.html\"";
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    catch (std::exception err)
    {
        std::cout << "Error in the JSON file" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return 0;
}