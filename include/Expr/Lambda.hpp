#pragma once

#include "Expr.hpp"
#include "ExprVisitor.hpp"
#include "Stmt/Stmt.hpp"
#include "Token.hpp"
#include <memory>
#include <vector>

class Lambda : public Expr {
public:
    Lambda(std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body);

    std::any accept(ExprVisitor<std::any>& visitor) const override;
    const std::vector<Token>& getParams() const { return params; }
    const std::vector<std::unique_ptr<Stmt>>& getBody() const { return body; }
    std::vector<std::unique_ptr<Stmt>>&& takeBody() { return std::move(body); }
private:
    std::vector<Token> params;
    std::vector<std::unique_ptr<Stmt>> body;
};  

