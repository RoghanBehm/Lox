#include "Expr/Get.hpp"
#include <memory>

Get::Get(std::unique_ptr<Expr> object, Token name)
    : object(std::move(object)), name(name) {}

std::any Get::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitGet(*this);
}
