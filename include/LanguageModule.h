#ifndef LANGUAGE_MODULE_H
#define LANGUAGE_MODULE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <ctime>

enum Language { ENGLISH=0, JAPANESE, HUNGARIAN };   ///< Included languages.

namespace Lang {
    enum CaptionType { PRESS=0, START, EXIT_TO_MENU, PAUSE, CONTINUE, LVL1, LVL2, CAT_MARIO, NONE };    ///< Included caption types.
}

/** 
 * @brief Single quote (necessary for translation when changing language).
 */
struct Quote {
    std::string quote;  ///< Caption of the quote.
    size_t quoteIndex;  ///< Index of the quote.
};

/**
 * @brief Contains all the translations of a single language.
 */
class LanguageModule {
private:
    std::vector<std::string> captions;  ///< Contains the translations of the captions.
    std::vector<Quote> quotes;  ///< Contains the translated quotes.
public:
    /**
     * Creates a new GameObject with the given boxes and texture.
     * @param path Path to the file containing the translations.
     * @param isRemoved Represents an object beeing destroyed.
     * @see Quote
     */
    LanguageModule(std::string path);

    /**
     * @param index Index of the quote.
     * @see Quote
     * @return Returns the translation at the given index.
     */
    Quote GetQuote(size_t index) const;

    /**
     * @see Quote
     * @return Returns the a random quote.
     */
    Quote GetRandomQuote() const;

    /**
     * @param caption Type of caption that should be translated.
     * @return Returns the translation of the caption.
     */
    std::string GetTranslation(Lang::CaptionType caption) const;

    /**
     * Destructor.
     */
    ~LanguageModule();
};

#endif // LANGUAGE_MODULE_H