#ifndef LANGUAGE_MODULE_H
#define LANGUAGE_MODULE_H

#include <iostream>
#include <string>
#include <vector>

enum Language { ENGLISH=0, JAPANESE, HUNGARIAN };

namespace Lang {
    enum CaptionType { PRESS=0, START, EXIT_TO_MENU, PAUSE, CONTINUE, LVL1, LVL2, CAT_MARIO };
}

class LanguageModule {
private:
    std::vector<std::string> captions;
    std::vector<std::string> quotes;
public:
    LanguageModule(const char* path);
    std::string getRandomQuote() const;
    std::string getTranslation(Lang::CaptionType caption) const;
    ~LanguageModule();
};

#endif // LANGUAGE_MODULE_H