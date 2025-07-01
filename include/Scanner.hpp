#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"
#include "token_type.hpp"
#include "Lox.hpp"

class Scanner {
    private:
        const std::string source;
        Lox& lox;
        std::vector<Token> tokens;
        std::size_t start = 0;
        std::size_t current = 0;
        int line = 1;
        bool isAtEnd();
        char advance();
        void addToken(TokenType type);
        void addToken(TokenType type, std::any literal);
        void scanToken();
        void identifier();
        bool match(char expected);
        char peek();
        char peekNext();
        bool isAlpha(char c);
        bool isAlphaNumeric(char c);
        void string();
        bool isDigit(char c);
        void number();

        // Reserved words
        static inline const std::unordered_map<std::string, TokenType> keywords {
            {"and",    TokenType::AND},
            {"class",  TokenType::CLASS},
            {"else",   TokenType::ELSE},
            {"false",  TokenType::FALSE},
            {"for",    TokenType::FOR},
            {"fun",    TokenType::FUN},
            {"if",     TokenType::IF},
            {"nil",    TokenType::NIL},
            {"or",     TokenType::OR},
            {"print",  TokenType::PRINT},
            {"return", TokenType::RETURN},
            {"super",  TokenType::SUPER},
            {"this",   TokenType::THIS},
            {"true",   TokenType::TRUE},
            {"var",    TokenType::VAR},
            {"while",  TokenType::WHILE},
            {"break", TokenType::BREAK}
        };

    public: 
        Scanner(std::string source, Lox& lox);
        std::vector<Token> scanTokens();
};