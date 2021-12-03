#pragma once

#include <exception>

class BaseException : public std::exception
{
public:
    BaseException(const char* reason)
    {
        this->reason = reason;
    }

    virtual const char* what() const override
    {
        return reason;
    }

private:
    const char* reason;
};
