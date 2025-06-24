#include "AstPrinter.hpp"
#include <string>
#include "Expr/ExprVisitor.hpp"
#include "Expr/Expr.hpp"
#include "Expr/Binary.hpp"
#include "Expr/Grouping.hpp"
#include "Expr/Literal.hpp"
#include "Expr/Unary.hpp"
#include "Expr/Comma.hpp"
#include "Expr/Ternary.hpp"
#include "to_string.hpp"

std::string AstPrinter::print(const Expr& expr) {
    return std::any_cast<std::string>(expr.accept(*this));
}
std::any AstPrinter::visitBinary(const Binary& expr) {
    return parenthesize(expr.getOp().lexeme, expr.getLeftExpr(), expr.getRightExpr());
}
std::any AstPrinter::visitGrouping(const Grouping& expr) {
    return parenthesize("group", expr.getExpr());
}
std::any AstPrinter::visitLiteral(const Literal& expr) {
    if (!expr.getLiteral().has_value()) return "nil";
    return anyToString(expr.getLiteral());
}
std::any AstPrinter::visitUnary(const Unary& expr) {
    return parenthesize(expr.getOp().lexeme, expr.getRightExpr());
}
std::any AstPrinter::visitAssign(const Assign& expr) {
    return {};
}

std::any AstPrinter::visitCall(const Call& expr) {
    return {};
}

std::any AstPrinter::visitLogical(const Logical& expr) {
    return {};
}

std::any AstPrinter::visitVar(const Var& expr) {
    return {};
}

std::any AstPrinter::visitComma(const Comma& expr) {
    return {};
}

std::any AstPrinter::visitTernary(const Ternary& expr) {
    return {};
}

std::string AstPrinter::parenthesize(const std::string& name, const Expr& a) {
    return "(" + name + " " + print(a) + ")";
}

std::string AstPrinter::parenthesize(const std::string& name, const Expr& a, const Expr& b) {
    return "(" + name + " " + print(a) + " " + print(b) + ")";
}

std::string AstPrinter::rpn(const std::string& name, const Expr& a) {
    return print(a) + " " + name;
}

std::string AstPrinter::rpn(const std::string& name, const Expr& a, const Expr& b) {
    return print(a) + " " + print(b) + " " + name;
}


