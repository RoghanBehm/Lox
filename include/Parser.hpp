#include <functional>
#include <memory>
#include <vector>
#include "Expr/Expr.hpp"
#include "Token.hpp"
#include "Lox.hpp"

class Parser {
    private:
        Lox& lox;
        const std::vector<Token> tokens;
        int current = 0;
        std::unique_ptr<Expr> expression();
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
        Token consume(TokenType type, std::string message);
        void synchronize();

        std::unique_ptr<Expr> laparse(std::function<std::unique_ptr<Expr>()> op_type, TokenType a, TokenType b);
        class ParseError : public std::runtime_error {
            public:
                ParseError() : std::runtime_error("") {}
        };

        ParseError error(Token token, std::string message) const;
    public:
        Parser(std::vector<Token> tokens, Lox& lox);
        std::unique_ptr<Expr> parse();
};