#pragma once
#include "token_type.hpp"
#include <any>
#include <string>

class Token {
    private:

    
    public:
        const std::any literal;
        const TokenType type;
        std::string toString();
        Token(TokenType type, std::string lexeme, std::any literal, int line);
        const std::string lexeme;
        const int line;
};