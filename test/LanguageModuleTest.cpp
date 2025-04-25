# include <iostream>

#include "gtest_lite.h"
#include "LanguageModule.h"

#define COUNT 100

int main() {
    GTINIT(std::cin); // Csak C(J)PORTA működéséhez kell
    
    /***** Checking the Constructor *****/

    // Checks for throwing error for non-existing file
    TEST(OpenFile, FileNotFound) {
        EXPECT_ANY_THROW(LanguageModule("Non-existing file"));
    } END

    // Checks for not throwing error for the English config file
    TEST(OpenFile, English) {
        EXPECT_NO_THROW(LanguageModule("../res/lang/English.txt"));
    } END

    // Checks for not throwing error for the Japanese config file
    TEST(OpenFile, Japanese) {
        EXPECT_NO_THROW(LanguageModule("../res/lang/Japanese.txt"));
    } END

    // Checks for not throwing error for the Hungarian config file
    TEST(OpenFile, Hungarian) {
        EXPECT_NO_THROW(LanguageModule("../res/lang/Hungarian.txt"));
    } END


    /***** Checking Quotes *****/
    // It's enough to check the first and the last quote,
    // because the constuctor ensures that the count will match.

    // Checks the first quote
    TEST(Quote, FirstQuote) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_NO_THROW(english.getQuote(0));
        EXPECT_STREQ("Retreat, hell! We just got here.", english.getQuote(0).quote.c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_NO_THROW(japanese.getQuote(0));
        EXPECT_STREQ("撤退、地獄！着いたばかりだ", japanese.getQuote(0).quote.c_str());

        LanguageModule hungrian("../res/lang/Hungarian.txt");
        EXPECT_NO_THROW(hungrian.getQuote(0));
        EXPECT_STREQ("Visszavonulás, a pokolba! Most értünk ide.", hungrian.getQuote(0).quote.c_str());
    } END

    // Checks the last quote
    TEST(Quote, LastQuote) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_NO_THROW(english.getQuote(COUNT - 1));
        EXPECT_STREQ("Hooooo, that was close!", english.getQuote(COUNT - 1).quote.c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_NO_THROW(japanese.getQuote(COUNT - 1));
        EXPECT_STREQ("フーッ、危なかった！", japanese.getQuote(COUNT - 1).quote.c_str());

        LanguageModule hungrian("../res/lang/Hungarian.txt");
        EXPECT_NO_THROW(hungrian.getQuote(COUNT - 1));
        EXPECT_STREQ("Húúúúú, ez közel volt!", hungrian.getQuote(COUNT - 1).quote.c_str());
    } END

    // Checks for throws (It must be valid quote, if previous tests pass, so there is no need to check for that)
    TEST(Quote, getRandomQuote) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_NO_THROW(english.getRandomQuote());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_NO_THROW(japanese.getRandomQuote());

        LanguageModule hungrian("../res/lang/Hungarian.txt");
        EXPECT_NO_THROW(hungrian.getRandomQuote());
    } END


    /***** Checking getTranslation *****/

    // Checks for correct translation of PRESS
    TEST(getTranslation, PRESS) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Press any key to continue...", english.getTranslation(Lang::PRESS).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("キーを押して続ける...", japanese.getTranslation(Lang::PRESS).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Nyomjon meg egy gombot a folytatáshoz...", hungarian.getTranslation(Lang::PRESS).c_str());
    } END

    // Checks for correct translation of START
    TEST(getTranslation, START) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Start Game", english.getTranslation(Lang::START).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("試合開始", japanese.getTranslation(Lang::START).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Játék indítása", hungarian.getTranslation(Lang::START).c_str());
    } END

    // Checks for correct translation of EXIT_TO_MENU
    TEST(getTranslation, EXIT_TO_MENU) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Exit to Menu", english.getTranslation(Lang::EXIT_TO_MENU).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("メニューを終了する", japanese.getTranslation(Lang::EXIT_TO_MENU).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Kilépés a menübe", hungarian.getTranslation(Lang::EXIT_TO_MENU).c_str());
    } END

    // Checks for correct translation of PAUSE
    TEST(getTranslation, PAUSE) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Press button \"P\" while playing to pause game.", english.getTranslation(Lang::PAUSE).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("Pボタンを押してゲームを中断する。", japanese.getTranslation(Lang::PAUSE).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Játszás közben nyomja \"P\"-t a megállításhoz.", hungarian.getTranslation(Lang::PAUSE).c_str());
    } END

    // Checks for correct translation of CONTINUE
    TEST(getTranslation, CONTINUE) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Continue", english.getTranslation(Lang::CONTINUE).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("続ける", japanese.getTranslation(Lang::CONTINUE).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Folytatás", hungarian.getTranslation(Lang::CONTINUE).c_str());
    } END

    // Checks for correct translation of LVL1
    TEST(getTranslation, LVL1) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Level 1", english.getTranslation(Lang::LVL1).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("レベル1", japanese.getTranslation(Lang::LVL1).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("1. Szint", hungarian.getTranslation(Lang::LVL1).c_str());
    } END

    // Checks for correct translation of LVL2
    TEST(getTranslation, LVL2) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Level 2", english.getTranslation(Lang::LVL2).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("レベル2", japanese.getTranslation(Lang::LVL2).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("2. Szint", hungarian.getTranslation(Lang::LVL2).c_str());
    } END

    // Checks for correct translation of CAT_MARIO
    TEST(getTranslation, CAT_MARIO) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Cat Mario", english.getTranslation(Lang::CAT_MARIO).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("キャット・マリオ", japanese.getTranslation(Lang::CAT_MARIO).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Macska Márió", hungarian.getTranslation(Lang::CAT_MARIO).c_str());
    } END

    GTEND(std::cerr); // Csak C(J)PORTA működéséhez kell
    return 0;
}