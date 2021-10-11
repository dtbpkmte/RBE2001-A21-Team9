#include <cstdio>

class Test {
    public:
        const static unsigned int MAX_NUM_TESTS = 20;

    public:
        Test();
        ~Test();

        bool addTest(char *, int, bool (*f)(void));
        //bool addTest(char *, int, void (*)(void));
        void runAllTests();
        void printResults(bool verbose=true);

    private:
        unsigned int numTests = 0;
        unsigned int numPassed = 0;
        char * testnames[MAX_NUM_TESTS];
        int testname_lens[MAX_NUM_TESTS];
        bool (*tests[MAX_NUM_TESTS]) (void);
        int results[MAX_NUM_TESTS] = {0};
};