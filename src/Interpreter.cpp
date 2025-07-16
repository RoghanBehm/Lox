#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "Interpreter.hpp"
#include "BreakException.hpp"
#include "LoxCallable.hpp"
#include "LoxInstance.hpp"
#include "LoxLambda.hpp"
#include "ReturnException.hpp"
#include "Expr/Var.hpp"
#include "Token.hpp"
#include "token_type.hpp"
#include "Lox.hpp"
#include "LoxClass.hpp"


Interpreter::Interpreter(Lox& lox) : lox(lox){
    globals = std::make_shared<Environment>();
    environment = globals;

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

void Interpreter::resolve(const Expr& expr, int depth) {
    locals[&expr] = depth;
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

std::any Interpreter::lookUpVariable(Token name, const Expr& expr) {

    if (locals.find(&expr) != locals.end()) {
        int distance = locals.at(&expr); 
        return environment->getAt(distance, name.lexeme);
    } else {
        return globals->get(name);
    }
}


// STATEMENTS
void Interpreter::visitBlock(const Block& stmt) {
    executeBlock(stmt.getStatements(), std::make_shared<Environment>(environment));
}

void Interpreter::visitExpression(const Expression& stmt) {
    evaluate(stmt.getExpr());
}

void Interpreter::visitIf(const If& stmt) {
    if (isTruthy(evaluate(stmt.getCondition()))) {
        execute(stmt.getThenBranch());
    } else if (stmt.hasElseBranch()) {
        execute(stmt.getElseBranch());
    }
}

void Interpreter::visitClass(const Class& stmt) {
    std::any superclass;
    std::shared_ptr<LoxClass> superclassClass = nullptr;

    if (stmt.getSuperclass() != nullptr) {
        superclass = evaluate(*stmt.getSuperclass());
        if (auto callable = std::any_cast<std::shared_ptr<LoxCallable>>(&superclass)) {
            superclassClass = std::dynamic_pointer_cast<LoxClass>(*callable);
        }

        if (!superclassClass) {
            throw RuntimeError(stmt.getSuperclass()->getName(), "Superclass must be a class.");
        }
    }
    
    environment->define(stmt.getName().lexeme, nullptr);

    if (stmt.getSuperclass() != nullptr) {
        environment = std::make_shared<Environment>(environment);
        environment->define("super", superclass);
    }

    auto classEnv = std::make_shared<Environment>(environment); 
    
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    for (const auto& method : stmt.getMethods()) {
        bool isInitialiser = method->getName().lexeme == "init";
        
        methods[method->getName().lexeme] =
            std::make_shared<LoxFunction>(*method, environment, isInitialiser);
    }

    std::shared_ptr<LoxCallable> klass = std::make_shared<LoxClass>(stmt.getName().lexeme, superclassClass, methods);

    if (stmt.getSuperclass() != nullptr) {
        environment = environment->getEnclosing();
    }

    environment->assign(stmt.getName(), klass);
}

void Interpreter::visitFunction(const Function& stmt) {
    std::shared_ptr<LoxCallable> function = std::make_shared<LoxFunction>(stmt, environment, false);
    environment->define(stmt.getName().lexeme, function);
}

void Interpreter::visitPrint(const Print& stmt) {
    std::any value = evaluate(stmt.getExpr());
    std::cout << stringify(value) << "\n";
}

void Interpreter::visitReturn(const Return& stmt) {
    std::any value = NULL;
    if (stmt.hasValue()) value = evaluate(stmt.getValue());

    throw ReturnException(value); 
}

void Interpreter::visitVarStmt(const VarStmt& stmt) {
    std::any value = NULL;
    if (stmt.hasInitializer()) {
        value = evaluate(stmt.getInitializer());
    }

    environment->define(stmt.getName().lexeme, value);
}

void Interpreter::visitWhile(const While& stmt) {
    while (isTruthy(evaluate(stmt.getCondition()))) {
       try {
        execute(stmt.getBody());
       } catch (const BreakException&) {
            break;
       }
    }
}

void Interpreter::visitBreak(const Break& stmt) {
    throw BreakException(stmt.getName());
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

std::any Interpreter::visitCall(const Call& expr) {
    std::any callee = evaluate(expr.getCallee());

    std::vector<std::any> arguments;
    for (const std::unique_ptr<Expr>& argument : expr.getArgruments()) {
        arguments.push_back(evaluate(*argument));
    }

    std::shared_ptr<LoxCallable> function;

    try {
        function = std::any_cast<std::shared_ptr<LoxCallable>>(callee);
    } catch (const std::bad_any_cast&) {
        try {
            auto loxFunction = std::any_cast<std::shared_ptr<LoxFunction>>(callee);
            function = loxFunction; 
        } catch (const std::bad_any_cast&) {
            throw RuntimeError(expr.getParen(), "Can only call functions and classes.");
        }
    }

    if (static_cast<int>(arguments.size()) != function->arity()) {
        throw RuntimeError(expr.getParen(), "Expected " +
            std::to_string(function->arity()) + " arguments but got " + 
            std::to_string(arguments.size()) + ".");
    }

    return function->call(*this, arguments);
}

std::any Interpreter::visitGet(const Get& expr) {
    std::any object = evaluate(expr.getObject());
    if (object.type() == typeid(std::shared_ptr<LoxInstance>)) {
        auto instance = std::any_cast<std::shared_ptr<LoxInstance>>(object);
        return instance->get(expr.getName());
    }
    throw RuntimeError(expr.getName(), "Only instances have properties.");
}


std::any Interpreter::visitLiteral(const Literal& expr) {
    return expr.getLiteral();
}

std::any Interpreter::visitLogical(const Logical& expr) {
    std::any left = evaluate(expr.getLeftExpr());

    if (expr.getOp().type == TokenType::OR) {
        if (isTruthy(left)) return left;
    } else {
        if (!isTruthy(left)) return left;
    }

    return evaluate(expr.getRightExpr());
}

std::any Interpreter::visitSet(const Set& expr) {
    std::any object = evaluate(expr.getObject());
    
    if (!(object.type() == typeid(std::shared_ptr<LoxInstance>))) {
        throw RuntimeError(expr.getName(),
             "Only instances have fields.");
    }

    auto instance = std::any_cast<std::shared_ptr<LoxInstance>>(object);
    std::any value = evaluate(expr.getValue());
    instance->set(expr.getName(), value);

    return value;
}

std::any Interpreter::visitSuper(const Super& expr) {
    int distance = locals.at(&expr);
    std::any superclassClass = environment->getAt(distance, "super");
    auto callable = std::any_cast<std::shared_ptr<LoxCallable>>(superclassClass);
    auto superclass = std::dynamic_pointer_cast<LoxClass>(callable);
    auto method = superclass->findMethod(expr.getMethod().lexeme);

    auto object = environment->getAt(distance - 1, "this");
    auto instance = std::any_cast<std::shared_ptr<LoxInstance>>(object);


    if (!method) {
        throw RuntimeError(expr.getMethod(),
            "Undefined property '" + expr.getMethod().lexeme + "'.");
    }
    return method->bind(instance);
}

std::any Interpreter::visitThis(const This& expr) {
    return lookUpVariable(expr.getKeyword(), expr);
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
    return lookUpVariable(expr.getName(), expr);
}

std::any Interpreter::visitAssign(const Assign& expr) {
    std::any value = evaluate(expr.getValue());
    
    if (locals.find(&expr) != locals.end()) {
        int distance = locals.at(&expr);
        environment->assignAt(distance, expr.getName(), value);
    } else {
        globals->assign(expr.getName(), value);
    }
    
 

    return value;
}

std::any Interpreter::visitLambda(const Lambda& expr) {
    auto params = expr.getParams(); 
    auto body = const_cast<Lambda&>(expr).takeBody();

    std::shared_ptr<LoxLambda> lambda = std::make_shared<LoxLambda>(
        std::move(params),
        std::move(body),
        environment
    );


    return std::static_pointer_cast<LoxCallable>(lambda);
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

    if (obj.type() == typeid(double)) {
        double value = std::any_cast<double>(obj);
        if (std::isinf(value)) return false;
        if (std::isnan(value)) return false;
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

    if (object.type() == typeid(std::shared_ptr<LoxInstance>)) {
        auto instance = std::any_cast<std::shared_ptr<LoxInstance>>(object);
        return instance->toString();
    }

    return std::any_cast<std::string>(object);
}

//////////////



// UNIMPLEMENTED VISIT METHODS

// EXPRESSIONS

std::any Interpreter::visitComma(const Comma& expr) {
    // implement this
    return {};
}

std::any Interpreter::visitTernary(const Ternary& expr) {
    // implement this
    return {};
}
//////////////
