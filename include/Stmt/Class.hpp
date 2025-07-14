#pragma once

#include "Stmt.hpp"
#include "StmtVisitor.hpp"
#include "Token.hpp"
#include "Stmt/Function.hpp"
#include <memory>
#include <vector>


class Class : public Stmt {
public:
    Class(Token name, /*std::unique_ptr<Var> superclass,*/ std::vector<std::unique_ptr<Function>> methods);
    void accept(StmtVisitor<void>& visitor) const override;

    const Token& getName() const { return name; };
    /*const std::unique_ptr<Var>& getSuperclass() const { return superclass; }*/
    const std::vector<std::unique_ptr<Function>>& getMethods() const { return methods; }

private:
    Token name;
    /*std::unique_ptr<Var> superclass;*/
    std::vector<std::unique_ptr<Function>> methods;
};
