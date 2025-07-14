#include <memory>
#include "Resolver.hpp"
#include "Expr/Lambda.hpp"
#include "Stmt/VarStmt.hpp"
#include "Stmt/Block.hpp"
#include "Stmt/Function.hpp"
#include "Stmt/Class.hpp"
#include "Expr/Var.hpp"
#include "Expr/Assign.hpp"
#include "Expr/Get.hpp"
#include "Expr/Set.hpp"

Resolver::Resolver(Interpreter& interpreter, Lox& lox) : interpreter(interpreter), lox(lox) {};

void Resolver::visitBlock(const Block& stmt) {
    beginScope();
    resolve(stmt.getStatements());
    endScope(); 
}

void Resolver::visitClass(const Class& stmt) {
    declare(stmt.getName());
    define(stmt.getName());

    for (const auto& method : stmt.getMethods()) {
        FunctionType declaration = FunctionType::METHOD;
        resolveFunction(*method, declaration);
    }
}

void Resolver::visitVarStmt(const VarStmt& stmt) {
    declare(stmt.getName());
    if (stmt.hasInitializer()) {
        resolve(stmt.getInitializer());
    }
    define(stmt.getName());
}

void Resolver::visitFunction(const Function& stmt) {
    declare(stmt.getName());
    define(stmt.getName());

    resolveFunction(stmt, FunctionType::FUNCTION);
}

void Resolver::visitExpression(const Expression& stmt) {
    resolve(stmt.getExpr());
}

void Resolver::visitIf(const If& stmt) {
    resolve(stmt.getCondition());
    resolve(stmt.getThenBranch());
    if (stmt.hasElseBranch()) resolve(stmt.getElseBranch());
}

void Resolver::visitPrint(const Print& stmt) {
    resolve(stmt.getExpr());
}

void Resolver::visitReturn(const Return& stmt) {
    if (currentFunction == FunctionType::NONE) {
        lox.error(stmt.getKeyword(), "Can't return from top-level code.");
    }
    
    if (stmt.hasValue()) {
        resolve(stmt.getValue());
    }
}

void Resolver::visitWhile(const While& stmt) {
    resolve(stmt.getCondition());
    resolve(stmt.getBody());
}

void Resolver::visitBreak(const Break& stmt) {
    return;
}

std::any Resolver::visitVar(const Var& expr) {

    if (!scopes.empty() 
        && scopes.back().count(expr.getName().lexeme)  // Check if key exists in map
        && scopes.back().at(expr.getName().lexeme) == false) 
    {
        lox.error(expr.getName(), "Can't read local variable in its own initializer.");
    }
    resolveLocal(expr, expr.getName());
    return {}; // Ideally method should be void
}

std::any Resolver::visitAssign(const Assign& expr) {
    resolve(expr.getValue());
    resolveLocal(expr, expr.getName());
    return {};
}

std::any Resolver::visitBinary(const Binary& expr) {
    resolve(expr.getLeftExpr());
    resolve(expr.getRightExpr());
    return {};
}

std::any Resolver::visitCall(const Call& expr) {
    resolve(expr.getCallee());

    for (const auto &argument : expr.getArgruments()) {
        resolve(*argument);
    }

    return {};
}

std::any Resolver::visitGet(const Get& expr) {
    resolve(expr.getObject());
    return {};
}

std::any Resolver::visitSet(const Set& expr) {
    resolve(expr.getValue());
    resolve(expr.getObject());
    return {};
}

std::any Resolver::visitGrouping(const Grouping& expr) {
    resolve(expr.getExpr());
    return {};
}

std::any Resolver::visitLiteral(const Literal& expr) {
    return {};
}

std::any Resolver::visitLogical(const Logical& expr) {
    resolve(expr.getLeftExpr());
    resolve(expr.getRightExpr());
    return {};
}

std::any Resolver::visitUnary(const Unary& expr) {
    resolve(expr.getRightExpr());
    return {};
}

std::any Resolver::visitLambda(const Lambda& expr) {
    resolveLambda(expr);
    return {};
}

std::any Resolver::visitComma(const Comma& expr) {
    return {}; // Not implemented
}

std::any Resolver::visitTernary(const Ternary& expr) {
    return{}; // Not implemented
}


void Resolver::resolve(const std::vector<std::unique_ptr<Stmt>>& statements) {
    for (const auto& statement : statements) {
        resolve(*statement);
    }
}

void Resolver::resolve(const Stmt& stmt) {
    stmt.accept(*this);
}

void Resolver::resolve(const Expr& expr) {
    expr.accept(*this);
}

void Resolver::resolveFunction(const Function& function, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token param : function.getParams()) {
        declare(param);
        define(param);
    }
    resolve(function.getBody());
    endScope();
    currentFunction = enclosingFunction;
}

void Resolver::resolveLambda(const Lambda& function) {
    beginScope();
    for (Token param : function.getParams()) {
        declare(param);
        define(param);
    }
    resolve(function.getBody());
    endScope();
}



void Resolver::beginScope() {
    scopes.emplace_back(); // Empty unordered_map<string, bool>
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(Token name) {
    if (scopes.empty()) return;
        auto& scope = scopes.back();
    if (scope.contains(name.lexeme)) {
        lox.error(name, "Already a variable with this name in this scope.");
    }
    scope[name.lexeme] = false;
}

void Resolver::define(Token name) {
    if (scopes.empty()) return;
    scopes.back()[name.lexeme] = true;
}

void Resolver::resolveLocal(const Expr& expr, Token name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].find(name.lexeme) != scopes[i].end()) {
            interpreter.resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}