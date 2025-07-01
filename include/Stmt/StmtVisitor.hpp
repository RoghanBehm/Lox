#pragma once

class Block;
class Expression;
class If;
class Function;
class Print;
class Return;
class VarStmt;
class While;
class Break;

template<typename R>
class StmtVisitor {
public:
    ~StmtVisitor() = default;

    virtual R visitBlock(const Block& stmt) = 0;
    virtual R visitExpression(const Expression& stmt) = 0;
    virtual R visitIf(const If& stmt) = 0;
    virtual R visitFunction(const Function& stmt) = 0;
    virtual R visitPrint(const Print& stmt) = 0;
    virtual R visitReturn(const Return& stmt) = 0;
    virtual R visitVarStmt(const VarStmt& stmt) = 0;
    virtual R visitWhile(const While& stmt) = 0;
    virtual R visitBreak(const Break& stmt) = 0;
};
