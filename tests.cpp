#include <cstdio>
#include <assert.h>

#include "lexer.hpp"
#include "parser.hpp"

int main()
{
    struct TestCase
    {
        std::string data;
        int result;
    };

    TestCase tests[] = {
        { "1 + 1", 2 },
        { "1-1", 0 },
        { "2/1", 2 },
        { "1*2", 2 },
        { "  1+\t1", 2 },
        { "3 * 6 + 2 / 1", 20 },
        { "1 + 3 + 4 * 5", 24 },
        { "6 / 3 * 2 + 1 * 7 + 2", 13 },
        { "6^2 * 3", 108 },
        { "(1 + 3) * 10", 40 },
        { "4 * 2 + (6 + 1)", 15 }
    };

    for (auto& test : tests)
    {
        std::vector<LexerToken> tokens;
        Lexer::Parse(test.data, tokens);
        auto start = Parser::Parse(tokens);
        if (start == nullptr)
        {
            assert(0);
            return 1;
        }

        auto result = start->Solve();
        if (result != test.result)
        {
            printf("%s != %i (was %i)\n", test.data.c_str(), test.result, result);
            return 1;
        }

        printf("%s = %i\n", test.data.c_str(), result);
    }


    return 0;
}
