#pragma once

#include <memory>
#include "Stmt.hpp"
#include "StmtVisitor.hpp"
#include "Expr/Expr.hpp"
#include "Token.hpp"

class VarStmt : public Stmt {
public:
    VarStmt(Token name, std::unique_ptr<Expr> expr);
    void accept(StmtVisitor<void>& visitor) const override;

    const Token& getName() const { return name; }

    bool hasInitializer() const { return expr != nullptr; }
    const Expr& getInitializer() const { return *expr; }

private:
    Token name;
    std::unique_ptr<Expr> expr;
};
