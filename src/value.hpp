#pragma once

#include <cstdint>

class Value
{
public:
    Value(int64_t value);

    int64_t GetValueAsInt64() const { return value; }

private:
    int64_t value;
};

Value operator+(const Value& l, const Value& r);
Value operator-(const Value& l, const Value& r);
Value operator*(const Value& l, const Value& r);
Value operator/(const Value& l, const Value& r);
Value operator^(const Value& l, const Value& r);
bool operator==(const Value& l, const Value& r);
bool operator!=(const Value& l, const Value& r);
