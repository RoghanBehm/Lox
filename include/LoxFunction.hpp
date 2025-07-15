#pragma once
#include "Environment.hpp"
#include "LoxCallable.hpp"
#include "Stmt/Function.hpp"
#include <memory>
class LoxInstance;

class LoxFunction : public LoxCallable {
public:
    LoxFunction(const Function& declaration, std::shared_ptr<Environment> closure, bool isInitialiser);


    std::any call(Interpreter& interpreter, const std::vector<std::any>& arguments) const override;
    int arity() const override;
    std::string toString() const override;
    std::shared_ptr<LoxFunction> bind(std::shared_ptr<LoxInstance> instance);
private:
    const Function& declaration;
    std::shared_ptr<Environment> closure;
    bool isInitialiser;


    
};