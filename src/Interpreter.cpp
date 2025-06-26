#include "Interpreter.hpp"
#include "Token.hpp"
#include "token_type.hpp"
#include <iostream>
#include "Lox.hpp"

Interpreter::Interpreter(Lox& lox) : lox(lox){}

void Interpreter::interpret(const Expr& expression) {
    try {
        std::any value = evaluate(expression);
        std::cout << stringify(value) << "\n";
    } catch (RuntimeError error) {
        lox.runtimeError(error);
    }
}

std::any Interpreter::evaluate(const Expr& expr) {
    return expr.accept(*this);
}

std::any Interpreter::visitBinary(const Binary& expr) {
    std::any left = evaluate(expr.getLeftExpr());
    std::any right = evaluate(expr.getRightExpr());

    switch (expr.getOp().type) {
        case TokenType::GREATER:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) > std::any_cast<double>(right);
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        case TokenType::LESS:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) < std::any_cast<double>(right);
        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
        case TokenType::MINUS:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        case TokenType::PLUS:
            if (left.type() == typeid(double) && right.type() == typeid(double)) {
                return std::any_cast<double>(left) + std::any_cast<double>(right);
            }

            if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            }

            if (left.type() == typeid(std::string) || right.type() == typeid(std::string)) {
                return stringify(left) + stringify(right);
            }
            throw RuntimeError(expr.getOp(), "Operands must be two numbers or two strings.");
        case TokenType::SLASH:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        case TokenType::STAR:
            checkNumberOperands(expr.getOp(), left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        case TokenType::BANG_EQUAL: return !isEqual(left, right);
        case TokenType::EQUAL_EQUAL: return isEqual(left, right);
        default: break;
    }

    return NULL;
}

std::any Interpreter::visitLiteral(const Literal& expr) {
    return expr.getLiteral();
}

std::any Interpreter::visitGrouping(const Grouping& expr) {
    return evaluate(expr.getExpr());
}

std::any Interpreter::visitUnary(const Unary& expr) {
    std::any right = evaluate(expr.getRightExpr());

    switch (expr.getOp().type) {
        case TokenType::MINUS:
            checkNumberOperand(expr.getOp(), right);
            return -std::any_cast<double>(right);
        case TokenType::BANG:
            return !isTruthy(right);
        default: break;
    }

    return NULL;
}

void Interpreter::checkNumberOperand(Token op, std::any operand) {
    if (operand.type() == typeid(double)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token op, std::any left, std::any right) {
    if (left.type() == typeid(double) && right.type() == typeid(double)) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

bool Interpreter::isTruthy(std::any obj) {
    if (!obj.has_value()) return false;

    if (obj.type() == typeid(bool)) {
        return std::any_cast<bool>(obj);
    }

    return true;
}

bool Interpreter::isEqual(std::any a, std::any b) {
    if (!a.has_value() && !b.has_value()) return true;
    if (!a.has_value()) return false;
    if (a.type() == typeid(bool)) return compare<bool>(a, b);
    if (a.type() == typeid(double)) return compare<double>(a, b);
    if (a.type() == typeid(std::string)) return compare<std::string>(a, b);

    // Unsupported comparison
    return false;
}

std::string Interpreter::stringify(std::any object) {
    if (!object.has_value()) return "nil";

    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object) ? "true" : "false";
    }
    if (object.type() == typeid(double)) {
        double val = std::any_cast<double>(object);
        std::string text = std::to_string(val);
        if (text.find('.') != std::string::npos) {
            while (text.back() == '0') text.pop_back();
            if (text.back() == '.') text.pop_back();
        }
        return text;
    }

    return std::any_cast<std::string>(object);
}



std::any Interpreter::visitAssign(const Assign& expr) {
    // implement this
}

std::any Interpreter::visitCall(const Call& expr) {
    // implement this
}

std::any Interpreter::visitLogical(const Logical& expr) {
    // implement this
}

std::any Interpreter::visitVar(const Var& expr) {
    // implement this
}

std::any Interpreter::visitComma(const Comma& expr) {
    // implement this
}

std::any Interpreter::visitTernary(const Ternary& expr) {
    // implement this
}
