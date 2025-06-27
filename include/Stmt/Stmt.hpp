#pragma once

#include "Stmt/StmtVisitor.hpp"
#include <any>

class Stmt {
public:
    virtual ~Stmt() = default;

    virtual void accept(StmtVisitor<void>& visitor) const = 0;
};