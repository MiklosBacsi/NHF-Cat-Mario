#ifndef LANGUAGE_MODULE_H
#define LANGUAGE_MODULE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

enum Language { ENGLISH=0, JAPANESE, HUNGARIAN };

namespace Lang {
    enum CaptionType { PRESS=0, START, EXIT_TO_MENU, PAUSE, CONTINUE, LVL1, LVL2, CAT_MARIO, NONE };
}

// Necessary for translation when changing language
struct Quote {
    std::string quote;
    size_t quoteIndex;
};

class LanguageModule {
private:
    std::vector<std::string> captions;
    std::vector<Quote> quotes;
public:
    LanguageModule(std::string path);
    Quote GetQuote(size_t index) const;
    Quote GetRandomQuote() const;
    std::string GetTranslation(Lang::CaptionType caption) const;
    ~LanguageModule();
};

#endif // LANGUAGE_MODULE_H