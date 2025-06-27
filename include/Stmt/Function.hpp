#pragma once

#include "Stmt.hpp"
#include "StmtVisitor.hpp"
#include "Token.hpp"
#include <memory>
#include <vector>


class Function : public Stmt {
public:
    Function(Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body);
    void accept(StmtVisitor<void>& visitor) const override;

    const Token& getName() const { return name; };
    const std::vector<Token>& getParams() const { return params; }
    const std::vector<std::unique_ptr<Stmt>>& getBody() const { return body; }

private:
    Token name;
    std::vector<Token> params;
    std::vector<std::unique_ptr<Stmt>> body;
};
