#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

#include "LanguageModule.h"

LanguageModule::LanguageModule(const char* path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Unable to open language file.");
    }

    int captionCount;
    file >> captionCount;
    file.ignore(); // Ignore newline after the caption count

    // Read captions
    std::string line;
    for (int i = 0; i < captionCount; ++i) {
        std::getline(file, line);
        captions.push_back(line);
    }

    int quoteCount;
    file >> quoteCount;
    file.ignore(); // Ignore newline after the quote count

    // Read quotes
    for (int i = 0; i < quoteCount; ++i) {
        std::getline(file, line);
        quotes.push_back(line);
    }

    // Generate random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

std::string LanguageModule::getRandomQuote() const {
    return quotes[std::rand() % quotes.size()];
}

std::string LanguageModule::getTranslation(Lang::CaptionType capiton) const {
    return captions[capiton];
}

LanguageModule::~LanguageModule() {
    std::cout << "~LanguageModule Dtor" << std::endl;
}