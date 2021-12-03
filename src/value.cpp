#include "value.hpp"
#include "parser.hpp"

#include <cmath>

Value::Value(int64_t value)
{
    this->value = value;
}

Value operator+(const Value& l, const Value& r)
{
    return Value(l.GetValueAsInt64() + r.GetValueAsInt64());
}

Value operator-(const Value& l, const Value& r)
{
    return Value(l.GetValueAsInt64() - r.GetValueAsInt64());
}

Value operator*(const Value& l, const Value& r)
{
    return Value(l.GetValueAsInt64() * r.GetValueAsInt64());
}

Value operator/(const Value& l, const Value& r)
{
    auto lv = l.GetValueAsInt64();
    auto lr = r.GetValueAsInt64();

    if (lr == 0)
    {
        throw SolveException("Cannot divide by zero.");
    }

    if (lv % lr != 0)
    {
        throw SolveException("Cannot solve division that results in fractional numbers.");
    }

    return Value(lv / lr);
}

Value operator^(const Value& l, const Value& r)
{
    return Value((int64_t)pow(l.GetValueAsInt64(), r.GetValueAsInt64()));
}

bool operator==(const Value& l, const Value& r)
{
    return l.GetValueAsInt64() == r.GetValueAsInt64();
}

bool operator!=(const Value& l, const Value& r)
{
    return !(l == r);
}
