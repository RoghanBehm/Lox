#pragma once

#include <any>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Environment.hpp"
#include "Expr/Expr.hpp"
#include "Expr/ExprVisitor.hpp"
#include "Expr/Grouping.hpp"
#include "Expr/Literal.hpp"
#include "Expr/Unary.hpp"
#include "Expr/Binary.hpp"
#include "Expr/Call.hpp"
#include "Expr/Assign.hpp"
#include "Expr/Logical.hpp"
#include "Expr/Lambda.hpp"
#include "Expr/Get.hpp"
#include "Expr/Set.hpp"
#include "Expr/This.hpp"
#include "Stmt/Stmt.hpp"
#include "Stmt/StmtVisitor.hpp"
#include "Stmt/Expression.hpp"
#include "Stmt/Print.hpp"
#include "Stmt/Return.hpp"
#include "Stmt/VarStmt.hpp"
#include "Stmt/While.hpp"
#include "Stmt/Block.hpp"
#include "Stmt/If.hpp"
#include "Stmt/Break.hpp"
#include "Stmt/Class.hpp"

class Lox; 

class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<void> {
public:
    Interpreter(Lox& lox);
    virtual ~Interpreter() = default;
    void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);
    void executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements, std::shared_ptr<Environment> environment);
    std::any evaluate(const Expr& expr);
    std::string stringify(std::any object);
    void resolve(const Expr& expr, int depth);
    std::shared_ptr<Environment> globals = std::make_shared<Environment>();


    // Expr visit methods
    std::any visitLiteral(const Literal& expr) override;
    std::any visitGrouping(const Grouping& expr) override;
    std::any visitUnary(const Unary& expr) override;
    std::any visitBinary(const Binary& expr) override;
    std::any visitCall(const Call& expr) override;
    std::any visitGet(const Get& expr) override;
    std::any visitAssign(const Assign& expr) override;
    std::any visitLogical(const Logical& expr) override;
    std::any visitSet(const Set& expr) override;
    std::any visitThis(const This& expr) override;
    std::any visitVar(const Var& expr) override;
    std::any visitComma(const Comma& expr) override;
    std::any visitTernary(const Ternary& expr) override;
    std::any visitLambda(const Lambda& expr) override;

    // Stmt visit methods
    void visitExpression(const Expression& stmt) override;
    void visitPrint(const Print& stmt) override;
    void visitBlock(const Block& stmt) override;
    void visitClass(const Class& stmt) override;
    void visitFunction(const Function& stmt) override;
    void visitIf(const If& stmt) override;
    void visitReturn(const Return& stmt) override;
    void visitVarStmt(const VarStmt& stmt) override;
    void visitWhile(const While& stmt) override;
    void visitBreak(const Break& stmt) override;
    

private:
    std::shared_ptr<Environment> environment;
    bool isTruthy(std::any obj);
    bool isEqual(std::any a, std::any b);
    template<typename T>
    bool compare(const std::any& a, const std::any& b) {
        return std::any_cast<T>(a) == std::any_cast<T>(b);
    }
    void checkNumberOperand(Token op, std::any operand);
    void checkNumberOperands(Token op, std::any left, std::any right);
    void checkDivideByZero(Token op, std::any right);
    std::unordered_map<const Expr*, int> locals;
    void execute(const Stmt& stmt);
    std::any lookUpVariable(Token name, const Expr& expr);
    Lox& lox;
};




