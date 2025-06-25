#include "Token.hpp"
#include <stdexcept>

class RuntimeError : public std::runtime_error {
    public:
        const Token token;

    RuntimeError(const Token& token, const std::string& message)
        : std::runtime_error("[line " + std::to_string(token.line) + "] Error at '" + token.lexeme + "': " + message + "\n"), token(token){}
};