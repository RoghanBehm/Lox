#pragma once

#include <memory>
#include "Stmt.hpp"
#include "StmtVisitor.hpp"
#include "Expr/Expr.hpp"

class If : public Stmt {
public:
    If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch);
    void accept(StmtVisitor<void>& visitor) const override;

    const Expr& getCondition() const { return *condition; }
    const Stmt& getThenBranch() const { return *thenBranch; }
    bool hasElseBranch() const { return elseBranch != nullptr; }
    const Stmt& getElseBranch() const { return *elseBranch; }

private:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
};