#pragma once

#include <memory>
#include "StmtVisitor.hpp"
#include "Stmt.hpp"
#include "Expr/Expr.hpp"


class Expression : public Stmt {
public:
    Expression(std::unique_ptr<Expr> expr);
    void accept(StmtVisitor<void>& visitor) const override;

    const Expr& getExpr() const { return *expr; }

private:
    std::unique_ptr<Expr> expr;
};
