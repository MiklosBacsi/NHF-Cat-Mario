#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include "LanguageModule.h"

LanguageModule::LanguageModule(std::string path) {
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Unable to open language file!");

    int captionCount;
    if (!(file >> captionCount))
        throw std::runtime_error("Failed to read caption count!");
    file.ignore(); // Ignore newline after the caption count

    // Read captions
    std::string line;
    for (int i = 0; i < captionCount; ++i) {
        if (!std::getline(file, line))
            throw std::runtime_error("Failed to read caption line " + std::to_string(i));
        captions.push_back(line);
    }

    int quoteCount;
    if (!(file >> quoteCount))
        throw std::runtime_error("Failed to read quote count!");
    file.ignore(); // Ignore newline after the quote count

    // Read quotes
    for (int i = 0; i < quoteCount; ++i) {
        if (!std::getline(file, line))
            throw std::runtime_error("Failed to read quote line " + std::to_string(i));    
        quotes.push_back(line);
    }

    // Generate random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

std::string LanguageModule::getRandomQuote() const {
    if (quotes.empty())
        throw std::runtime_error("No quotes available!");
    return quotes[std::rand() % quotes.size()];
}

std::string LanguageModule::getTranslation(Lang::CaptionType caption) const {
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