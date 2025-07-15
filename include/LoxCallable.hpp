#pragma once

#include <any>
#include <string>
#include <vector>

class Interpreter;

class LoxCallable {
public:
    virtual ~LoxCallable();
    virtual std::any call(Interpreter&, const std::vector<std::any>&) const = 0;
    virtual int arity() const = 0;
    virtual std::string toString() const = 0;
};