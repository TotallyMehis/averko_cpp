#include "lexer.hpp"


void Lexer::Parse(const std::string& data, std::vector<LexerToken>& tokens)
{
    CharType cur_char_type = CharType::Unknown;
    std::string cur_token = "";
    int cur_txt_pos = 0;

    auto append_to_list = [&](CharType char_type, int txt_pos)
    {
        if (!IsSameCharType(cur_char_type, char_type))
        {
            if (cur_char_type != CharType::Unknown && cur_char_type != CharType::Whitespace)
            {
                auto token = TokenToLexerToken(cur_token, cur_char_type, cur_txt_pos);
                tokens.push_back(token);
            }

            cur_token = "";
            cur_txt_pos = txt_pos;
            cur_char_type = char_type;
        }
    };


    auto length = data.length();
    for (size_t i = 0; i < length; i++)
    {
        auto cur = data[i];
        auto char_type = GetCharType(cur);

        append_to_list(char_type, i);

        cur_token += cur;
    }

    append_to_list(CharType::Unknown, length);
}

CharType Lexer::GetCharType(char c)
{
    if (c == ' ' || c == '\t')
    {
        return CharType::Whitespace;
    }

    if (c == '+')
    {
        return CharType::OperatorAdd;
    }

    if (c == '-')
    {
        return CharType::OperatorSubtract;
    }

    if (c == '*')
    {
        return CharType::OperatorMultiply;
    }

    if (c == '/')
    {
        return CharType::OperatorDivide;
    }

    if (c == '^')
    {
        return CharType::OperatorExponent;
    }

    if (c == '(')
    {
        return CharType::ParenthesisStart;
    }

    if (c == ')')
    {
        return CharType::ParenthesisEnd;
    }

    if ((c >= '0' && c <= '9') || c == '.' || c == ',')
    {
        return CharType::Value;
    }

    return CharType::Character;
}

bool Lexer::IsSameCharType(CharType cur_type, CharType new_type)
{
    if (cur_type == CharType::Unknown || new_type == CharType::Unknown)
    {
        return false;
    }

    return cur_type == new_type;
}

LexerToken Lexer::TokenToLexerToken(const std::string& token, CharType char_type, int text_pos)
{
    LexerToken lexer_token;

    lexer_token.text_pos = text_pos;

    switch (char_type)
    {
    case CharType::Value:
        if (token.find(',') != std::string::npos || token.find('.') != std::string::npos)
        {
            throw LexerException("Fractional numbers aren't allowed.");
        }

        lexer_token.type = TokenType::Constant;
        lexer_token.value = token;
        break;
    case CharType::Character:
        throw LexerException("Calculation cannot contain characters.");

        // TODO
        //lexer_token.type = TokenType::Variable;
        //lexer_token.value = token;
        break;
    case CharType::OperatorAdd:
        lexer_token.type = TokenType::OperatorAdd;
        break;
    case CharType::OperatorSubtract:
        lexer_token.type = TokenType::OperatorSubtract;
        break;
    case CharType::OperatorMultiply:
        lexer_token.type = TokenType::OperatorMultiply;
        break;
    case CharType::OperatorDivide:
        lexer_token.type = TokenType::OperatorDivide;
        break;
    case CharType::OperatorExponent:
        lexer_token.type = TokenType::OperatorExponent;
        break;
    case CharType::ParenthesisStart:
        lexer_token.type = TokenType::ParenthesisStart;
        break;
    case CharType::ParenthesisEnd:
        lexer_token.type = TokenType::ParenthesisEnd;
        break;
    default:
        throw LexerException("Unexpected character type!\n");
        break;
    }

    return lexer_token;
}
