#pragma once

#include <memory>
#include "Stmt.hpp"
#include "StmtVisitor.hpp"
#include "Expr/Expr.hpp"

class Print : public Stmt {
public:
    Print(std::unique_ptr<Expr> expr);
    void accept(StmtVisitor<void>& visitor) const override;

    const Expr& getExpr() const { return *expr; }

private:
    std::unique_ptr<Expr> expr;
};

