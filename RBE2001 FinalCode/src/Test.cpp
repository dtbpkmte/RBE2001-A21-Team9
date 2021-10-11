#include "Test.h"

Test::Test() {}

Test::~Test() {}

bool Test::addTest(char * nameptr, int namelen, bool (*testfunc)(void)) {
    if (numTests < MAX_NUM_TESTS) {
        tests[numTests] = testfunc;
        testnames[numTests] = nameptr;
        testname_lens[numTests] = namelen;
        ++numTests;
        return true;
    }
    return false;
}

// bool Test::addTest(char * nameptr, int namelen, void (*testfunc)(void)) {
//     if (numTests < MAX_NUM_TESTS) {
//         tests[numTests] = testfunc;
//         testnames[numTests] = nameptr;
//         testname_lens[numTests] = namelen;
//         ++numTests;
//         return true;
//     }
//     return false;
// }

void Test::runAllTests() {
    for (auto i = 0; i < numTests; ++i) {
        results[i] = (*tests[i])();
        if (results[i]) ++numPassed;
    }
}

void Test::printResults(bool verbose) {
    printf("%u tests passed out of %u tests.\n");
    if (verbose) {
        printf("Full report:\n");
        for (auto i = 0; i < numTests; ++i) {
            printf("Test #%u: %s - %s\n", i, testnames[i], results[i] ? "Passed" : "Failed");
        }
    }
    
}