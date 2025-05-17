/** @file TestMain.cpp
  * @brief This program tests the modules of the game Cat Mario
  *  
  * This program uses the gtest_lite.h library to test some of
  * the game's modules by calling their test functions.
  *
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifdef MY_TEST

# include <iostream>

#include "gtest_lite.h"
#include "RigidBodyTest.hpp"
#include "LanguageModuleTest.hpp"

int main() {
    GTINIT(std::cin); // Csak C(J)PORTA működéséhez kell
    
    TEST_RigidBody();
    TEST_LanguageModule();

    GTEND(std::cerr); // Csak C(J)PORTA működéséhez kell
    return 0;
}

#endif // MY_TEST