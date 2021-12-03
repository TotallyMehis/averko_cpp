#include <iostream>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "options.hpp"


static void OptionsMenu();
static void CalculatorMenu();
static void ClearScreen();
static void ReadString(std::string& value);
static void WaitForEnter();

int main()
{
    if (OptionsState::GetOption("start_calculator_menu")) {
        CalculatorMenu();
    }

    auto exit = false;
    while (!exit)
    {
        ClearScreen();
        std::cout << "========================" << std::endl;
        std::cout << "Main menu" << std::endl;
        std::cout << std::endl;
        std::cout << "1. Calculator" << std::endl;
        std::cout << "2. Options" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "========================" << std::endl;

        std::cout << "Please enter the action you want: ";

        std::string value;
        ReadString(value);

        if (!value.length())
        {
            continue;
        }

        auto option = atoi(value.c_str());

        switch (option)
        {
        case 1:
            CalculatorMenu();
            break;
        case 2:
            OptionsMenu();
            break;
        case 3:
            exit = true;
            break;
        default:
            std::cout << "Unknown option: " << value << std::endl;
            WaitForEnter();
            break;
        }
    }

    return 0;
}

static void ClearScreen()
{
    for (int i = 0; i < 32; i++)
    {
        std::cout << std::endl;
    }
}

static void ReadString(std::string& value)
{
    // Way to complicate reading input. Fuck this garbage library.
    //std::cin >> value;
    //std::cin.ignore();
    std::getline(std::cin, value);
}

static void WaitForEnter()
{
    std::cin.ignore();
}

static void OptionsMenu()
{
    while (true)
    {
        auto save_to_file = OptionsState::GetOption("save_calculations_to_file");
        auto start_calculator_menu = OptionsState::GetOption("start_calculator_menu");

        ClearScreen();
        std::cout << "========================" << std::endl;
        std::cout << "Options menu" << std::endl;
        std::cout << std::endl;
        std::cout << "Enter the number of the option you want to change." << std::endl;
        std::cout << "Press Enter by itself to exit." << std::endl;
        std::cout << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "1. Output calculations to file: " << (save_to_file ? "ON" : "OFF") << std::endl;
        std::cout << "2. Start program at calculator menu: " << (start_calculator_menu ? "ON" : "OFF") << std::endl;
        std::cout << "========================" << std::endl;

        std::cout << "Enter option number: ";

        std::string value;
        ReadString(value);

        if (!value.length())
        {
            break;
        }

        auto option = atoi(value.c_str());
        switch (option)
        {
        case 1:
            OptionsState::SetOption("save_calculations_to_file", !save_to_file);
            break;
        case 2:
            OptionsState::SetOption("start_calculator_menu", !start_calculator_menu);
            break;
        default:
            std::cout << "Unknown option: " << value << std::endl;
            WaitForEnter();
            break;
        }
    }
}

static void CalculatorMenu()
{
    ClearScreen();
    std::cout << "========================" << std::endl;
    std::cout << "Calculator" << std::endl;
    std::cout << std::endl;
    std::cout << "Write a calculation, press Enter and the program will output the answer." << std::endl;
    std::cout << "NOTE: Only calculations that contain and resolve to whole numbers are allowed." << std::endl;
    std::cout << "Press Enter by itself to exit." << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "5 * (2 + 10)" << std::endl;
    std::cout << "2 - (-2)" << std::endl;
    std::cout << "4 / 2 * 4^2 - 10" << std::endl;
    std::cout << "========================" << std::endl;

    while (true)
    {
        std::cout << "Enter calculation: ";


        std::string calculation;
        ReadString(calculation);

        if (!calculation.length())
        {
            break;
        }

        try
        {
            std::vector<LexerToken> tokens;
            Lexer::Parse(calculation, tokens);

            auto start = Parser::Parse(tokens);

            auto answer = start->Solve().GetValueAsInt64();

            std::cout << "Answer is: " << answer << std::endl;

            if (OptionsState::GetOption("save_calculations_to_file"))
            {
                std::ofstream stream("calculations.txt", std::ios_base::app);
                if (stream.is_open())
                {
                    stream << calculation << " = " << answer << std::endl;
                }
            }
        }
        catch (const ParsingException& e)
        {
            std::cout << "Error in calculation: " << e.what() << std::endl;
        }
        catch (const SolveException& e)
        {
            std::cout << "Error solving: " << e.what() << std::endl;
        }
        catch (const LexerException& e)
        {
            std::cout << "Error lexing: " << e.what() << std::endl;
        }
    }
}
