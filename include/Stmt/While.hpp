#pragma once

#include <memory>
#include "Stmt.hpp"
#include "StmtVisitor.hpp"
#include "Expr/Expr.hpp"

class While : public Stmt {
public:
    While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);
    void accept(StmtVisitor<void>& visitor) const override;

    const Expr& getCondition() const { return *condition; }
    const Stmt& getBody() const { return *body; }

private:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};
