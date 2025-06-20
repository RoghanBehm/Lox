#pragma once
#include "token_type.hpp"
#include <any>
#include <string>

class Token {
    private:
        const TokenType type;
        const std::string lexeme;
        const std::any literal;
        const int line;
    
    public:
        std::string toString();
        Token(TokenType type, std::string lexeme, std::any literal, int line);
};