#include "ConverterJSON.h"
#include "SearchEngine.h"
#include "InvertedIndex.h"

int main() {

    ConverterJSON converterJSON;
    InvertedIndex invertedIndex;
    SearchServer searchServer(invertedIndex);

    std::ifstream configJson ("config.json");
    std::ifstream requestsJson ("requests.json");
    std::ifstream answersJson ("answers.json");

    if (!configJson)
    {
        std::cout << "File \"config.json\" not found" << std::endl;
        return 0;
    }

    if (!requestsJson)
    {
        std::cout << "File \"requests.json\" not found" << std::endl;
        return 0;
    }

    if (!answersJson)
    {
        std::cout << "File \"answers.json\" not found" << std::endl;
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
            return 0;
        }

        if (requests.size() == 0) {
            std::cout << "Error! Requests are missing" << std::endl;

            std::ofstream HTMLFile("result.html");
            HTMLFile << "<p> Error! Requests are missing </p>" << std::endl;
            HTMLFile.close();
            return 0;
        }

        auto answers = searchServer.completionAnswers();

        converterJSON.putAnswers(answers);

        createHTML(answers, requests, converterJSON, listDocResult);


        std::ifstream read("config.json");
        nlohmann::json jsonRead;
        read >> jsonRead;
        read.close();

        std::cout << "======================================" << std::endl << jsonRead["config"]["name"] << std::endl << "======================================" << std::endl;

        std::cout << "Successfully! The result is in file \"result.html\"";
    }

    catch (std::exception err)
    {
        std::cout << "Error in the JSON file" << std::endl;
    }
    return 0;
}