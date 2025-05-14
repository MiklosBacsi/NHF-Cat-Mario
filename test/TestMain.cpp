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