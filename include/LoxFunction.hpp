#include "Environment.hpp"
#include "Expr/ExprVisitor.hpp"
#include "LoxCallable.hpp"
#include "Stmt/Function.hpp"
#include <memory>

class LoxFunction : public LoxCallable {
public:
    LoxFunction(const Function& declaration, std::shared_ptr<Environment> closure);


    std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) override;
    int arity() const override;
    std::string toString() const override;

private:
    const Function& declaration;
 
    std::shared_ptr<Environment> closure;
};