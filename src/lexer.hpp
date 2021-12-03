#pragma once

#include <string>
#include <vector>

#include "exceptions.h"

enum struct TokenType
{
    OperatorAdd = 0, // +
    OperatorSubtract, // -
    OperatorMultiply, // *
    OperatorDivide, // /
    OperatorExponent, // ^

    Constant, // Any constant value

    Variable, // x, y or z

    ParenthesisStart, // (
    ParenthesisEnd, // )
};

enum struct CharType
{
    Unknown = 0,

    Whitespace,

    Character,

    OperatorAdd,
    OperatorSubtract,
    OperatorMultiply,
    OperatorDivide,
    OperatorExponent,

    ParenthesisStart,
    ParenthesisEnd,

    Value
};

struct LexerToken
{
    int text_pos;

    TokenType type;

    std::string value;
};


class Lexer
{
public:
    static void Parse(const std::string& data, std::vector<LexerToken>& tokens);

private:
    static CharType GetCharType(char c);
    static LexerToken TokenToLexerToken(const std::string& token, CharType char_type, int text_pos);
    static bool IsSameCharType(CharType cur_type, CharType new_type);
};

class LexerException : public BaseException
{
public:
    LexerException(const char* reason) : BaseException(reason) {}
};
