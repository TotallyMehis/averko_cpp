#pragma once

#include <memory>
#include <vector>

#include "exceptions.h"
#include "lexer.hpp"
#include "value.hpp"


enum struct Priority : int
{
    // PEMDAS
    AddSubtract,
    MultiplyDivide,
    Exponent,
    ParenthesesFunctions,

    // Constants always have the top priority.
    Constant
};

class Expression
{
public:
    Expression(Priority priority);
    virtual ~Expression();

    virtual Value Solve() = 0;

    Priority GetPriority() const { return priority; }


    Expression* parent;

    Expression* left;
    Expression* right;

private:
    Priority priority;
};

class Parser
{
public:
    static std::shared_ptr<Expression> Parse(const std::vector<LexerToken>& tokens);

private:
    static Expression* InnerParse(const std::vector<LexerToken>& tokens);
};



class ParsingException : public BaseException
{
public:
    ParsingException(const char* reason) : BaseException(reason) {}
};

class SolveException : public BaseException
{
public:
    SolveException(const char* reason) : BaseException(reason) {}
};
