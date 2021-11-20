#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"

int main()
{
    std::cout << "========================" << std::endl;
    std::cout << "Calculator" << std::endl;
    std::cout << std::endl;
    std::cout << "Write a calculation, press Enter and the program will output the answer." << std::endl;
    std::cout << "Press Enter to exit." << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "5 * (2 + 10)" << std::endl;
    std::cout << "4 / 2 * 4^2 - 10" << std::endl;
    std::cout << "========================" << std::endl;

    while (true)
    {
        std::cout << "Enter calculation: ";

        std::string calculation;
        std::getline(std::cin, calculation);

        if (!calculation.length()) {
            break;
        }

        std::vector<LexerToken> tokens;
        Lexer::Parse(calculation, tokens);
        auto start = Parser::Parse(tokens);
        
        auto answer = start->Solve();

        std::cout << "Answer is: " << answer << std::endl;
    }

    return 0;
}
