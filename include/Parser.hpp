#include <functional>
#include <memory>
#include <vector>
#include "Expr/Expr.hpp"
#include "Stmt/Stmt.hpp"
#include "Token.hpp"
#include "Lox.hpp"

class Parser {
    private:
        Lox& lox;
        const std::vector<Token> tokens;
        bool repl;
        int current = 0;

        std::unique_ptr<Expr> equality();
        bool match(std::initializer_list<TokenType> types);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        std::unique_ptr<Expr> comparison();
        std::unique_ptr<Expr> term();
        std::unique_ptr<Expr> factor();
        std::unique_ptr<Expr> unary();
        std::unique_ptr<Expr> primary();
        // std::unique_ptr<Expr> comma();
        std::unique_ptr<Expr> ternary();
        std::unique_ptr<Expr> assignment();
        Token consume(TokenType type, std::string message);
        void synchronize();
        std::unique_ptr<Expr> laparse(std::function<std::unique_ptr<Expr>()> op_type, std::initializer_list<TokenType> types);

        std::unique_ptr<Stmt> statement();
        std::unique_ptr<Stmt> printStatement();
        std::unique_ptr<Stmt> expressionStatement();
        std::vector<std::unique_ptr<Stmt>> block();
        std::unique_ptr<Stmt> declaration();
        std::unique_ptr<Stmt> varDeclaration();

        
    public:
        Parser(std::vector<Token> tokens, Lox& lox, bool repl);
        std::vector<std::unique_ptr<Stmt>> parse();

        class ParseError : public std::runtime_error {
            public:
                ParseError() : std::runtime_error("") {}
        };
        ParseError error(Token token, std::string message) const;
        std::unique_ptr<Expr> expression();

};