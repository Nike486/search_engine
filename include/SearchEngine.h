﻿#pragma once

#include "InvertedIndex.h"
#include <thread>
#include <mutex>


struct RelativeIndex{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

void recordWords (std::vector<std::vector<std::vector<Entry>>> &VVVEntry, std::vector<std::vector<std::string>> VVString, std::vector<std::vector<Entry>> VEntry, int i, InvertedIndex _index, std::vector<int> &counter);

void createHTML (std::vector<std::vector<std::pair<int, float>>> answers, std::vector<std::basic_string<char>> requests, ConverterJSON converterJSON, std::vector<std::basic_string<char>> listDocResult);


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
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input);

    std::vector<std::vector<std::pair<int, float>>> completionAnswers();

private:
    ConverterJSON converterJson;
    InvertedIndex _index;
};