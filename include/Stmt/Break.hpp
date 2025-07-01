#pragma once

#include "Stmt.hpp"
#include "StmtVisitor.hpp"
#include "Stmt/Stmt.hpp"
#include "Token.hpp"

class Break : public Stmt {
public:
    Break(Token name);
    void accept(StmtVisitor<void>& visitor) const override;

    const Token& getName() const { return name; }

private:
    Token name;
};

