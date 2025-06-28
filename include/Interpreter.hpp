#pragma once

#include <any>
#include <memory>
#include <vector>
#include "Environment.hpp"
#include "Expr/Expr.hpp"
#include "Expr/ExprVisitor.hpp"
#include "Expr/Grouping.hpp"
#include "Expr/Literal.hpp"
#include "Expr/Unary.hpp"
#include "Expr/Binary.hpp"
#include "Expr/Assign.hpp"
#include "Stmt/Stmt.hpp"
#include "Stmt/StmtVisitor.hpp"
#include "Stmt/Expression.hpp"
#include "Stmt/Print.hpp"
#include "Stmt/VarStmt.hpp"
#include "Stmt/Block.hpp"

class Lox; 

class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<void> {
public:
    Interpreter(Lox& lox);
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);
    void executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment);
    
    // Expr visit methods
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

    // Stmt visit methods
    void visitExpression(const Expression& stmt) override;
    void visitPrint(const Print& stmt) override;
    void visitBlock(const Block& stmt) override;
    void visitFunction(const Function& stmt) override;
    void visitIf(const If& stmt) override;
    void visitReturn(const Return& stmt) override;
    void visitVarStmt(const VarStmt& stmt) override;
    void visitWhile(const While& stmt) override;

private:
    std::shared_ptr<Environment> environment;
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
    void checkDivideByZero(Token op, std::any right);
    void execute(const Stmt& stmt);
    Lox& lox;
};




