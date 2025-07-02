#pragma once

#include "Interpreter.hpp"
#include "LoxCallable.hpp"
#include <chrono>

class ClockFunction : public LoxCallable {
public: 
    int arity() const override {
        return 0;
    }

    std::any call(Interpreter& interpreter, const std::vector<std::any>& argument) override {
        auto now = std::chrono::system_clock::now().time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(now).count() / 1000.0;
        return seconds;
    }

    std::string toString() const override {
        return "<native fn>";
    }
};