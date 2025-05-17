/** @file LanguageModuleTest.hpp
  * @brief This file contains tests for the module LanguageModule
  * 
  * For testing, it uses the gtest_lite.h library.
  * 
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

# include <iostream>

#include "gtest_lite.h"
#include "LanguageModule.h"

#define COUNT 100

void TEST_LanguageModule() {
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
        EXPECT_NO_THROW(english.GetQuote(0));
        EXPECT_STREQ("Retreat, hell! We just got here.", english.GetQuote(0).quote.c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_NO_THROW(japanese.GetQuote(0));
        EXPECT_STREQ("撤退、地獄！着いたばかりだ", japanese.GetQuote(0).quote.c_str());

        LanguageModule hungrian("../res/lang/Hungarian.txt");
        EXPECT_NO_THROW(hungrian.GetQuote(0));
        EXPECT_STREQ("Visszavonulás, a pokolba! Most értünk ide.", hungrian.GetQuote(0).quote.c_str());
    } END

    // Checks the last quote
    TEST(Quote, LastQuote) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_NO_THROW(english.GetQuote(COUNT - 1));
        EXPECT_STREQ("Hooooo, that was close!", english.GetQuote(COUNT - 1).quote.c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_NO_THROW(japanese.GetQuote(COUNT - 1));
        EXPECT_STREQ("フーッ、危なかった！", japanese.GetQuote(COUNT - 1).quote.c_str());

        LanguageModule hungrian("../res/lang/Hungarian.txt");
        EXPECT_NO_THROW(hungrian.GetQuote(COUNT - 1));
        EXPECT_STREQ("Húúúúú, ez közel volt!", hungrian.GetQuote(COUNT - 1).quote.c_str());
    } END

    // Checks for throws (It must be valid quote, if previous tests pass, so there is no need to check for that)
    TEST(Quote, GetRandomQuote) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_NO_THROW(english.GetRandomQuote());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_NO_THROW(japanese.GetRandomQuote());

        LanguageModule hungrian("../res/lang/Hungarian.txt");
        EXPECT_NO_THROW(hungrian.GetRandomQuote());
    } END


    /***** Checking GetTranslation *****/

    // Checks for correct translation of PRESS
    TEST(GetTranslation, PRESS) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Press any key to continue...", english.GetTranslation(Lang::PRESS).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("キーを押して続ける...", japanese.GetTranslation(Lang::PRESS).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Nyomjon meg egy gombot a folytatáshoz...", hungarian.GetTranslation(Lang::PRESS).c_str());
    } END

    // Checks for correct translation of START
    TEST(GetTranslation, START) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Start Game", english.GetTranslation(Lang::START).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("試合開始", japanese.GetTranslation(Lang::START).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Játék indítása", hungarian.GetTranslation(Lang::START).c_str());
    } END

    // Checks for correct translation of EXIT_TO_MENU
    TEST(GetTranslation, EXIT_TO_MENU) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Exit to Menu", english.GetTranslation(Lang::EXIT_TO_MENU).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("メニューに戻る", japanese.GetTranslation(Lang::EXIT_TO_MENU).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Kilépés a menübe", hungarian.GetTranslation(Lang::EXIT_TO_MENU).c_str());
    } END

    // Checks for correct translation of PAUSE
    TEST(GetTranslation, PAUSE) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Press button \"P\" while playing to pause game.", english.GetTranslation(Lang::PAUSE).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("Pボタンを押してゲームを中断する。", japanese.GetTranslation(Lang::PAUSE).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Játszás közben nyomja \"P\"-t a megállításhoz.", hungarian.GetTranslation(Lang::PAUSE).c_str());
    } END

    // Checks for correct translation of CONTINUE
    TEST(GetTranslation, CONTINUE) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Continue", english.GetTranslation(Lang::CONTINUE).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("続ける", japanese.GetTranslation(Lang::CONTINUE).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Folytatás", hungarian.GetTranslation(Lang::CONTINUE).c_str());
    } END

    // Checks for correct translation of LVL1
    TEST(GetTranslation, LVL1) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Level 1", english.GetTranslation(Lang::LVL1).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("レベル1", japanese.GetTranslation(Lang::LVL1).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("1. Szint", hungarian.GetTranslation(Lang::LVL1).c_str());
    } END

    // Checks for correct translation of LVL2
    TEST(GetTranslation, LVL2) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Level 2", english.GetTranslation(Lang::LVL2).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("レベル2", japanese.GetTranslation(Lang::LVL2).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("2. Szint", hungarian.GetTranslation(Lang::LVL2).c_str());
    } END

    // Checks for correct translation of CAT_MARIO
    TEST(GetTranslation, CAT_MARIO) {
        LanguageModule english("../res/lang/English.txt");
        EXPECT_STREQ("Cat Mario", english.GetTranslation(Lang::CAT_MARIO).c_str());

        LanguageModule japanese("../res/lang/Japanese.txt");
        EXPECT_STREQ("キャット・マリオ", japanese.GetTranslation(Lang::CAT_MARIO).c_str());

        LanguageModule hungarian("../res/lang/Hungarian.txt");
        EXPECT_STREQ("Macska Márió", hungarian.GetTranslation(Lang::CAT_MARIO).c_str());
    } END
}