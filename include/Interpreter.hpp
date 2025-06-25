#pragma once

#include <any>
#include "Expr/Expr.hpp"
#include "Expr/ExprVisitor.hpp"
#include "Expr/Grouping.hpp"
#include "Expr/Literal.hpp"
#include "Expr/Unary.hpp"
#include "Expr/Binary.hpp"
class Lox; // Forward declaration

class Interpreter : ExprVisitor<std::any> {
public:
    Interpreter(Lox& lox);
    void interpret(const Expr& expression);
    std::any visitLiteral(const Literal& expr) override;
    std::any visitGrouping(const Grouping& expr) override;
    std::any visitUnary(const Unary& expr) override;
    std::any visitBinary(const Binary& expr) override;
    std::any visitAssign(const Assign& expr) override;
    std::any visitCall(const Call& expr) override;
    std::any visitLogical(const Logical& expr) override;
    std::any visitVar(const Var& expr) override;
    std::any visitComma(const Comma& expr) override;
    std::any visitTernary(const Ternary& expr) override;
private:
    std::any evaluate(const Expr& expr);
    bool isTruthy(std::any obj);
    bool isEqual(std::any a, std::any b);
    std::string stringify(std::any object);
    template<typename T>
    bool compare(const std::any& a, const std::any& b) {
        return std::any_cast<T>(a) == std::any_cast<T>(b);
    }
    void checkNumberOperand(Token op, std::any operand);
    void checkNumberOperands(Token op, std::any left, std::any right);
    Lox& lox;
};




