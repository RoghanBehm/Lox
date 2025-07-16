#pragma once

class Assign;
class Binary;
class Call;
class Grouping;
class Logical;
class Literal;
class Unary;
class Var;
class Comma;
class Ternary;
class Lambda;
class Get;
class Set;
class This;
class Super;

template<typename R>
class ExprVisitor {
public:
    ~ExprVisitor() = default;

    virtual R visitAssign(const Assign& expr) = 0;
    virtual R visitBinary(const Binary& expr) = 0;
    virtual R visitCall(const Call& expr) = 0;
    virtual R visitGrouping(const Grouping& expr) = 0;
    virtual R visitLogical(const Logical& expr) = 0;
    virtual R visitLiteral(const Literal& expr) = 0;
    virtual R visitUnary(const Unary& expr) = 0;
    virtual R visitVar(const Var& expr) = 0;
    virtual R visitComma(const Comma& expr) = 0;
    virtual R visitTernary(const Ternary& expr) = 0;
    virtual R visitLambda(const Lambda& expr) = 0;
    virtual R visitGet(const Get& expr) = 0;
    virtual R visitSet(const Set& expr) = 0;
    virtual R visitThis(const This& expr) = 0;
    virtual R visitSuper(const Super& expr) = 0;
};