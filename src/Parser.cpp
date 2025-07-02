#include "Parser.hpp"
#include "Expr/Expr.hpp"
#include "Token.hpp"
#include "token_type.hpp"
#include "Expr/Binary.hpp"
#include "Expr/Unary.hpp"
#include "Expr/Literal.hpp"
#include "Expr/Grouping.hpp"
#include "Expr/Var.hpp"
#include "Expr/Assign.hpp"
#include "Expr/Logical.hpp"
#include "Expr/Call.hpp"
// #include "Expr/Comma.hpp"
// #include "Expr/Ternary.hpp"
#include "Stmt/If.hpp"
#include "Stmt/Print.hpp"
#include "Stmt/While.hpp"
#include "Stmt/Expression.hpp"
#include "Stmt/VarStmt.hpp"
#include "Stmt/Block.hpp"
#include "Stmt/Break.hpp"
#include "Stmt/Function.hpp"
#include "Stmt/Return.hpp"
#include <any>
#include <cstddef>
#include <memory>

// Constructor
Parser::Parser(std::vector<Token> tokens, Lox& lox, bool repl)
    : lox(lox), tokens(std::move(tokens)), repl(repl) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

// Parses left-associative binary operators
std::unique_ptr<Expr> Parser::laparse(std::function<std::unique_ptr<Expr>()> op_type, std::initializer_list<TokenType> types) {
    std::unique_ptr<Expr> expr = op_type();

    while (match(types)) {
        Token op = previous();
        std::unique_ptr<Expr> right = op_type();
        expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::expression() {
    return assignment();
}

std::unique_ptr<Stmt> Parser::declaration() {
    try {
        if (match({TokenType::FUN})) return function("function");
        if (match({TokenType::VAR})) return varDeclaration();

        return statement();
    } catch (const ParseError& error) {
        synchronize();
        return NULL;
    }
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::RETURN})) return returnStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::LEFT_BRACE})) return std::make_unique<Block>(block());
    if (match({TokenType::BREAK})) return breakStatement();

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::forStatement() {
    ldepth++;
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::unique_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) {
        initializer = NULL;
    } else if (match({TokenType::VAR})) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    std::unique_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    std::unique_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    std::unique_ptr<Stmt> body = statement();
    ldepth--;
    if (increment != nullptr) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(body));
        statements.push_back(std::make_unique<Expression>(std::move(increment)));
        body = std::make_unique<Block>(std::move(statements));
    }

    if (condition == nullptr) condition = std::make_unique<Literal>(true);
    body = std::make_unique<While>(std::move(condition), std::move(body));

    if (initializer != nullptr) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<Block>(std::move(statements));
    }

    return body;
}

std::unique_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '() after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    std::unique_ptr<Stmt> thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = NULL;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }

    return std::make_unique<If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<Print>(std::move(value));
}

std::unique_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::unique_ptr<Expr> value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<Return>(keyword, std::move(value));
}

std::unique_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::unique_ptr<Expr> initializer = NULL;
    if (match({TokenType::EQUAL})) initializer = expression();

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::whileStatement() {
    ldepth++;
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    std::unique_ptr<Stmt> body = statement();
    ldepth--;

    return std::make_unique<While>(std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::breakStatement() {
    if (ldepth == 0) throw error(previous(), "Cannot use 'break' outside of a loop.");
    Token name = previous();
    consume(TokenType::SEMICOLON, "Expect ';' after break statement.");
    
    return std::make_unique<Break>(std::move(name));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> expr = expression();
    if (repl) {
        if (check(TokenType::SEMICOLON)) advance();
    } else {
        consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    }
    return std::make_unique<Expression>(std::move(expr));
}

std::unique_ptr<Stmt> Parser::function(std::string kind) {
    Token name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Can't have more than 255 parameters.");
            }

            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

    consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::unique_ptr<Stmt>> body = block();
    return std::make_unique<Function>(name, parameters, std::move(body));
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Expr> Parser::assignment() {
    std::unique_ptr<Expr> expr = lor();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        std::unique_ptr<Expr> value = assignment();

        if (auto varExpr = dynamic_cast<Var*>(expr.get())) {
            return std::make_unique<Assign>(varExpr->getName(), std::move(value));
        } else {
            error(equals, "Invalid assignment target.");
        }
    }
    return expr;
}

std::unique_ptr<Expr> Parser::lor() {
    std::unique_ptr<Expr> expr = land();
    while (match({TokenType::OR})) {
        Token op = previous();
        std::unique_ptr<Expr> right = land();
        expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::land() {
    std::unique_ptr<Expr> expr = equality();

    while (match({TokenType::AND})) {
        Token op = previous();
        std::unique_ptr<Expr> right = equality();
        expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    return laparse([this]() { return comparison(); }, {TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL});
}

std::unique_ptr<Expr> Parser::comparison() {
    return laparse([this]() { return term(); }, {
    TokenType::GREATER,
    TokenType::GREATER_EQUAL,
    TokenType::LESS,
    TokenType::LESS_EQUAL
});
}

std::unique_ptr<Expr> Parser::term() {
    return laparse([this]() { return factor(); }, {TokenType::MINUS, TokenType::PLUS});
}

std::unique_ptr<Expr> Parser::factor() {
    return laparse([this]() { return unary(); }, {TokenType::SLASH, TokenType::STAR});
}

std::unique_ptr<Expr> Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        return std::make_unique<Unary>(op, std::move(right));
    }
    
    if (match({TokenType::STAR, TokenType::SLASH, TokenType::STAR})) {
        Token op = peek();
        error(op, "Expect left-side operand.");
        advance();
        return std::make_unique<Literal>(std::any{});
    }

    return call();
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> callee) {
    std::vector<std::unique_ptr<Expr>> arguments;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    return std::make_unique<Call>(std::move(callee), paren, std::move(arguments));
}

std::unique_ptr<Expr> Parser::call() {
    std::unique_ptr<Expr> expr = primary();

    while (true) {
            if (match({TokenType::LEFT_PAREN})) {
                expr = finishCall(std::move(expr));
            } else {
                break;
            }
    }

    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_unique<Literal>(false);
    if (match({TokenType::TRUE})) return std::make_unique<Literal>(true);
    if (match({TokenType::NIL})) return std::make_unique<Literal>(std::any{});

    if (match({TokenType::NUMBER, TokenType::STRING})) {
        return std::make_unique<Literal>(previous().literal);
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<Var>(previous());
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
