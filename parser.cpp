#include <cmath>

#include "parser.hpp"

Expression::Expression(Priority priority)
{
    parent = nullptr;
    left = nullptr;
    right = nullptr;

    this->priority = priority;
}

Expression::~Expression()
{
    if (left != nullptr)
    {
        delete left;
        left = nullptr;
    }

    if (right != nullptr)
    {
        delete right;
        right = nullptr;
    }
}

class ValueExpression : public Expression
{
public:
    ValueExpression(std::string value) : Expression(Priority::Constant)
    {
        this->value = atoi(value.c_str());
    }

    virtual int Solve() override
    {
        return value;
    }

    int value;
};

class ParenthesesExpression : public Expression
{
public:
    ParenthesesExpression(Expression* inner_expression) : Expression(Priority::ParenthesesFunctions)
    {
        this->inner_expression = inner_expression;
    }

    ~ParenthesesExpression()
    {
        if (inner_expression)
        {
            delete inner_expression;
            inner_expression = nullptr;
        }
    }

    virtual int Solve() override
    {
        return inner_expression->Solve();
    }

private:
    Expression* inner_expression;
};

class OperatorAddExpression : public Expression
{
public:
    OperatorAddExpression() : Expression(Priority::AddSubtract)
    {
    }

    virtual int Solve() override
    {
        return left->Solve() + right->Solve();
    }
};

class OperatorSubtractExpression : public Expression
{
public:
    OperatorSubtractExpression() : Expression(Priority::AddSubtract)
    {
    }

    virtual int Solve() override
    {
        return left->Solve() - right->Solve();
    }
};

class OperatorMultiplyExpression : public Expression
{
public:
    OperatorMultiplyExpression() : Expression(Priority::MultiplyDivide)
    {
    }

    virtual int Solve() override
    {
        return left->Solve() * right->Solve();
    }
};

class OperatorDivideExpression : public Expression
{
public:
    OperatorDivideExpression() : Expression(Priority::MultiplyDivide)
    {
    }

    virtual int Solve() override
    {
        return left->Solve() / right->Solve();
    }
};

class OperatorExponentExpression : public Expression
{
public:
    OperatorExponentExpression() : Expression(Priority::Exponent)
    {
    }

    virtual int Solve() override
    {
        return pow(left->Solve(), right->Solve());
    }
};

std::shared_ptr<Expression> Parser::Parse(const std::vector<LexerToken>& tokens)
{
    return std::shared_ptr<Expression>(InnerParse(tokens));
}

Expression* Parser::InnerParse(const std::vector<LexerToken>& tokens)
{
    Expression* cur_expr = nullptr;

    auto len = tokens.size();
    for (auto i = 0; i < len; i++)
    {
        auto& token = tokens[i];

        Expression* new_expr = nullptr;
        switch (token.type)
        {
        case TokenType::OperatorAdd:
            new_expr = new OperatorAddExpression();
            break;
        case TokenType::OperatorSubtract:
            new_expr = new OperatorSubtractExpression();
            break;
        case TokenType::OperatorMultiply:
            new_expr = new OperatorMultiplyExpression();
            break;
        case TokenType::OperatorDivide:
            new_expr = new OperatorDivideExpression();
            break;
        case TokenType::OperatorExponent:
            new_expr = new OperatorExponentExpression();
            break;
        case TokenType::Constant:
            new_expr = new ValueExpression(token.value);
            break;
        case TokenType::ParenthesisStart:
        {
            int level = 1;
            auto start = i + 1;
            auto end = start;

            // Find the end parenthesis
            for (end = start; end < len; end++)
            {
                auto& inner_token = tokens[end];

                if (inner_token.type == TokenType::ParenthesisStart)
                {
                    level++;
                }
                else if (inner_token.type == TokenType::ParenthesisEnd)
                {
                    level--;

                    if (level <= 0)
                    {
                        break;
                    }
                }
            }

            i = end;

            std::vector<LexerToken> inner_tokens(tokens.begin() + start, tokens.begin() + end);
            new_expr = new ParenthesesExpression(Parser::InnerParse(inner_tokens));
            break;
        }
        case TokenType::ParenthesisEnd:
            printf("Something went terribly wrong!\n");
            break;
        default:
            printf("Unimplemented token type %i!\n", token.type);
            break;
        }

        if (!new_expr)
        {
            continue;
        }

        auto cur = cur_expr;
        while (cur != nullptr)
        {
            auto parent = cur->parent;

            if (new_expr->GetPriority() > cur->GetPriority())
            {
                new_expr->parent = cur;
                new_expr->left = cur->right;
                cur->right = new_expr;
                break;
            }
            else if (parent == nullptr)
            {
                cur->parent = new_expr;
                new_expr->left = cur;
                break;
            }

            cur = parent;
        }

        cur_expr = new_expr;
    }

    // Walk all the way to the top.
    auto start = cur_expr;
    while (start != nullptr && start->parent != nullptr)
    {
        start = start->parent;
    }

    return start;
}
