#include "to_string.hpp"
#include <iomanip>

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        // Single-character tokens
        case TokenType::LEFT_PAREN:      return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:     return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:      return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:     return "RIGHT_BRACE";
        case TokenType::COMMA:           return "COMMA";
        case TokenType::DOT:             return "DOT";
        case TokenType::MINUS:           return "MINUS";
        case TokenType::PLUS:            return "PLUS";
        case TokenType::SEMICOLON:       return "SEMICOLON";
        case TokenType::SLASH:           return "SLASH";
        case TokenType::STAR:            return "STAR";

        // One or two character tokens
        case TokenType::BANG:            return "BANG";
        case TokenType::BANG_EQUAL:      return "BANG_EQUAL";
        case TokenType::EQUAL:           return "EQUAL";
        case TokenType::EQUAL_EQUAL:     return "EQUAL_EQUAL";
        case TokenType::GREATER:         return "GREATER";
        case TokenType::GREATER_EQUAL:   return "GREATER_EQUAL";
        case TokenType::LESS:            return "LESS";
        case TokenType::LESS_EQUAL:      return "LESS_EQUAL";

        // Literals
        case TokenType::IDENTIFIER:      return "IDENTIFIER";
        case TokenType::STRING:          return "STRING";
        case TokenType::NUMBER:          return "NUMBER";

        // Keywords
        case TokenType::AND:             return "AND";
        case TokenType::CLASS:           return "CLASS";
        case TokenType::ELSE:            return "ELSE";
        case TokenType::FALSE:           return "FALSE";
        case TokenType::FUN:             return "FUN";
        case TokenType::FOR:             return "FOR";
        case TokenType::IF:              return "IF";
        case TokenType::NIL:             return "NIL";
        case TokenType::OR:              return "OR";
        case TokenType::PRINT:           return "PRINT";
        case TokenType::RETURN:          return "RETURN";
        case TokenType::SUPER:           return "SUPER";
        case TokenType::THIS:            return "THIS";
        case TokenType::TRUE:            return "TRUE";
        case TokenType::VAR:             return "VAR";
        case TokenType::WHILE:           return "WHILE";

        case TokenType::EOF_:        return "EOF";

        default:                         return "IDK";
    }
}


std::string anyToString(const std::any& val) {
    if (!val.has_value()) return "nil";

    if (val.type() == typeid(double)) {
        double num = std::any_cast<double>(val);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << num;
        std::string str = oss.str();
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (!str.empty() && str.back() == '.') str.pop_back();
        return str;
    } else if (val.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(val));
    } else if (val.type() == typeid(bool)) {
        return std::any_cast<bool>(val) ? "true" : "false";
    } else if (val.type() == typeid(std::string)) {
        return std::any_cast<std::string>(val);
    }

    return "";
}
