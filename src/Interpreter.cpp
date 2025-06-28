#include "Interpreter.hpp"
#include "Expr/Var.hpp"
#include "Token.hpp"
#include "token_type.hpp"
#include <iostream>
#include "Lox.hpp"


Interpreter::Interpreter(Lox& lox) : lox(lox){
    environment = std::make_shared<Environment>();
}

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
    try {
        for (const auto& statement : statements) {
            execute(*statement);
        }
    } catch (RuntimeError error) {
        lox.runtimeError(error);
    }
}

std::any Interpreter::evaluate(const Expr& expr) {
    return expr.accept(*this);
}

void Interpreter::execute(const Stmt& stmt) {
    stmt.accept(*this);
}

void Interpreter::executeBlock(
    const std::vector<std::unique_ptr<Stmt>>& statements,
    std::shared_ptr<Environment> newEnvironment
) {
    std::shared_ptr<Environment> previous = environment;
    environment = newEnvironment;

    try {
        for (const auto& stmt : statements) {
            execute(*stmt);
        }
    } catch (...) {
        environment = previous;
        throw;
    }

    environment = previous;
}


void Interpreter::visitBlock(const Block& stmt) {
    executeBlock(stmt.getStatements(), std::make_shared<Environment>(environment));
}

// STATEMENTS
void Interpreter::visitExpression(const Expression& stmt) {
    evaluate(stmt.getExpr());
}

void Interpreter::visitPrint(const Print& stmt) {
    std::any value = evaluate(stmt.getExpr());
    std::cout << stringify(value) << "\n";
}

void Interpreter::visitVarStmt(const VarStmt& stmt) {
    std::any value = NULL;
    if (stmt.hasInitializer()) {
        value = evaluate(stmt.getInitializer());
    }

    environment->define(stmt.getName().lexeme, value);
}
//////////////

// EXPRESSIONS
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
            checkDivideByZero(expr.getOp(), right);
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

std::any Interpreter::visitVar(const Var& expr) {
    return environment->get(expr.getName());
}

std::any Interpreter::visitAssign(const Assign& expr) {
    std::any value = evaluate(expr.getValue());
    environment->assign(expr.getName(), value);
    return value;
}


//////////////

// HELPERS

void Interpreter::checkNumberOperand(Token op, std::any operand) {
    if (operand.type() == typeid(double)) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token op, std::any left, std::any right) {
    if (left.type() == typeid(double) && right.type() == typeid(double)) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

void Interpreter::checkDivideByZero(Token op, std::any right) {
    if (std::any_cast<double>(right) != 0.0) return;
    throw RuntimeError(op, "Cannot divide by zero.");
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

//////////////



// UNIMPLEMENTED VISIT METHODS

// EXPRESSIONS
std::any Interpreter::visitCall(const Call& expr) {
    // implement this
}

std::any Interpreter::visitLogical(const Logical& expr) {
    // implement this
}

std::any Interpreter::visitComma(const Comma& expr) {
    // implement this
}

std::any Interpreter::visitTernary(const Ternary& expr) {
    // implement this
}
//////////////

// STATEMENTS

void Interpreter::visitFunction(const Function& expr) {
    // implement this 
}

void Interpreter::visitIf(const If& expr) {
    // implement this
}

void Interpreter::visitReturn(const Return& expr) {
    // implement this
}

void Interpreter::visitWhile(const While& expr) {
    // implement this
}



//////////////