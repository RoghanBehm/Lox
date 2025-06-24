#include "Parser.hpp"
#include "Expr/Expr.hpp"
#include "token_type.hpp"
#include "Expr/Binary.hpp"
#include "Expr/Unary.hpp"
#include "Expr/Literal.hpp"
#include "Expr/Grouping.hpp"
// #include "Expr/Comma.hpp"
// #include "Expr/Ternary.hpp"
#include <memory>

// Constructor
Parser::Parser(std::vector<Token> tokens, Lox& lox)
    : lox(lox), tokens(tokens) {}

std::unique_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (const ParseError& error) {
        return NULL;
    }
}

// Parses left-associative binary operators
std::unique_ptr<Expr> Parser::laparse(std::function<std::unique_ptr<Expr>()> op_type, TokenType a, TokenType b) {
    std::unique_ptr<Expr> expr = op_type();

    while (match({a, b})) {
        Token op = previous();
        std::unique_ptr<Expr> right = op_type();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::expression() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {
    return laparse([this]() { return comparison(); }, TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL);
}

std::unique_ptr<Expr> Parser::comparison() {
    return laparse([this]() { return term(); }, TokenType::GREATER, TokenType::LESS_EQUAL);
}

std::unique_ptr<Expr> Parser::term() {
    return laparse([this]() { return factor(); }, TokenType::MINUS, TokenType::PLUS);
}

std::unique_ptr<Expr> Parser::factor() {
    return laparse([this]() { return unary(); }, TokenType::SLASH, TokenType::STAR);
}

std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Unary>(op, std::move(right));
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_unique<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_unique<Literal>(true);
    if (match({TokenType::NIL})) return std::make_unique<Literal>(std::any{});

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<Literal>(previous().literal);
    }

    if (match({TokenType::LEFT_PAREN})) {
        std::unique_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

/* // Comma operator
std::unique_ptr<Expr> Parser::comma() {
    std::unique_ptr<Expr> expr = equality();
    
    while (match({TokenType::COMMA})) {
        std::unique_ptr<Expr> right = equality();
        expr = std::make_unique<Comma>(std::move(expr), std::move(right));
    }
    return expr;
    
}
*/

/* // Ternary operator
std::unique_ptr<Expr> Parser::ternary() {
    std::unique_ptr<Expr> expr = equality();
    
    if (match({TokenType::QUESTION})) {
        std::unique_ptr<Expr> thenif = expression();
        consume(TokenType::COLON, "Expect : after expression.");
        std::unique_ptr<Expr> thenelse = expression();
        expr = std::make_unique<Ternary>(std::move(expr), std::move(thenif), std::move(thenelse));
    }
    return expr;
}
*/

bool Parser::match(std::initializer_list<TokenType> types) {
    for (auto &t : types) {
        if (check(t)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType type, std::string message) {
    if (check(type)) return advance();

    throw error(peek(), message);
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_;
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

Parser::ParseError Parser::error(Token token, std::string message) const {
    lox.error(token, message);
    return ParseError{};
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;

        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default: 
                break;
        }

        advance();
    }
}
