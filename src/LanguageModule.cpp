#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include "LanguageModule.h"

LanguageModule::LanguageModule(std::string path) {
    // Open file
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Unable to open language file!");

    // Read quote count
    size_t captionCount;
    if (!(file >> captionCount))
        throw std::runtime_error("Failed to read caption count!");
    file.ignore(); // Ignore newline after the caption count

    // Read captions
    std::string line;
    for (size_t i = 0; i < captionCount; ++i) {
        if (!std::getline(file, line))
            throw std::runtime_error("Failed to read caption line " + std::to_string(i));
        captions.push_back(line);
    }
    if (captions.size() != captionCount)
        throw std::runtime_error("Caption count does no match");
    
    // Read quote count
    size_t quoteCount;
    if (!(file >> quoteCount))
        throw std::runtime_error("Failed to read quote count!");
    file.ignore(); // Ignore newline after the quote count

    // Read quotes
    for (size_t i = 0; i < quoteCount; ++i) {
        if (!std::getline(file, line))
            throw std::runtime_error("Failed to read quote line " + std::to_string(i));    
        quotes.push_back({.quote = line, .quoteIndex = i});
    }
    if (captions.size() != captionCount)
        throw std::runtime_error("Caption count does no match");
    
    // Generate random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Quote LanguageModule::GetQuote(size_t index) const { return quotes.at(index); }

Quote LanguageModule::GetRandomQuote() const {
    if (quotes.empty())
        throw std::runtime_error("No quotes available!");
    
    return GetQuote(std::rand() % quotes.size());
}

std::string LanguageModule::GetTranslation(Lang::CaptionType caption) const {
    if (caption == Lang::NONE)
        return " ";
    if (caption >= captions.size())
        throw std::out_of_range("Caption index out of range!");
    return captions[caption];
}

LanguageModule::~LanguageModule() {
    #ifdef DTOR
    std::clog << "~LanguageModule Dtor" << std::endl;
    #endif
}