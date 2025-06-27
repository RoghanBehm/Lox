#pragma once

#include <memory>
#include <vector>
#include "Stmt.hpp"
#include "StmtVisitor.hpp"

class Block : public Stmt {
public:
    Block(std::vector<std::unique_ptr<Stmt>> statements);
    void accept(StmtVisitor<void>& visitor) const override;
    const std::vector<std::unique_ptr<Stmt>>& getStatements() const { return statements; }

private:
    std::vector<std::unique_ptr<Stmt>> statements;
};
