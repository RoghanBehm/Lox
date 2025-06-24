#include "Token.hpp"
#include "to_string.hpp"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : literal(std::move(literal)), type(type), lexeme(std::move(lexeme)), line(line) {}

std::string Token::toString() {
    return tokenTypeToString(type) + " " + lexeme + " " + anyToString(literal);
}
