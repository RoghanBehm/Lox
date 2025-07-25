#pragma once

#include <string>
#include <any>
#include "Expr/Expr.hpp"
#include "Expr/ExprVisitor.hpp"

class AstPrinter : public ExprVisitor<std::any> {
public:
    std::string print(const Expr& expr);

    std::any visitBinary(const Binary& expr) override;
    std::any visitGrouping(const Grouping& expr) override;
    std::any visitLiteral(const Literal& expr) override;
    std::any visitUnary(const Unary& expr) override;
    std::any visitAssign(const Assign& expr) override;
    std::any visitCall(const Call& expr) override;
    std::any visitLogical(const Logical& expr) override;
    std::any visitVar(const Var& expr) override;
    std::any visitComma(const Comma& expr) override;
    std::any visitTernary(const Ternary& expr) override;

private:
    std::string parenthesize(const std::string& name, const Expr& a);
    std::string parenthesize(const std::string& name, const Expr& a, const Expr& b);
    std::string rpn(const std::string& name, const Expr& a);
    std::string rpn(const std::string& name, const Expr& a, const Expr& ab);
};
int printer();
