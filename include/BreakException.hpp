#pragma once

#include "Token.hpp"

class BreakException : public std::exception {
public:
    const Token token;

    BreakException(const Token& token)
        : token(token) {}

    const char* what() const noexcept override {
        return "Break statement encountered";
    }
};




