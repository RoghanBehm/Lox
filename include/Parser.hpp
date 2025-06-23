#include <memory>
#include <vector>
#include "Expr/Expr.hpp"
#include "Token.hpp"

class Parser {
    private:
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
    public:
        Parser(std::vector<Token> tokens) : tokens(tokens) {}
};