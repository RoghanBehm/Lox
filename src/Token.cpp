#include "Token.hpp"
#include "to_string.hpp"

Token::Token(TokenType type, std::string lexeme, std::any literal, int line)
    : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

std::string Token::toString() {
    return tokenTypeToString(type) + " " + lexeme + " " + anyToString(literal);
}
