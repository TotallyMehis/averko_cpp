#include <cstdio>
#include <assert.h>

#include "lexer.hpp"
#include "parser.hpp"
#include "value.hpp"

int main()
{
    struct TestCase
    {
        std::string data;
        Value result;
        bool expect_failure;
    };

    TestCase tests[] =
    {
        // These should pass.
        { "1 + 1", 2, false },
        { "1-1", 0, false },
        { "2/1", 2, false },
        { "1*2", 2, false },
        { "  1+\t1", 2, false },
        { "3 * 6 + 2 / 1", 20, false },
        { "1 + 3 + 4 * 5", 24, false },
        { "6 / 3 * 2 + 1 * 7 + 2", 13, false },
        { "6^2 * 3", 108, false },
        { "(1 + 3) * 10", 40, false },
        { "4 * 2 + (6 + 1)", 15, false },
        { "2 * (-2)", -4, false },
        { "-1", -1, false },
        { "+1", 1, false },

        // Failures.
        { "()", 0, true },
        { "(", 0, true },
        { ")", 0, true },
        { "_", 0, true },
    };

    for (auto& test : tests)
    {
        try
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

            if (test.expect_failure)
            {
                printf("%s expected failure!\n", test.data.c_str());
                return 1;
            }

            if (result != test.result)
            {
                printf("%s != %i (was %i)\n", test.data.c_str(), test.result.GetValueAsInt64(), result.GetValueAsInt64());
                return 1;
            }

            printf("%s = %i\n", test.data.c_str(), result);
        }
        catch (std::exception e)
        {
            if (!test.expect_failure)
            {
                printf("%s unexpected failure! (%s)\n", test.data.c_str(), e.what());
                return 1;
            }
        }
    }


    return 0;
}
