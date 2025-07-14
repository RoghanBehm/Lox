#include "Expr/Assign.hpp"
#include "Expr/Expr.hpp"
#include "Interpreter.hpp"
#include "Stmt/Stmt.hpp"
#include "Lox.hpp"
#include <memory>
#include <string>
#include <unordered_map>

class Resolver : public ExprVisitor<std::any>, public StmtVisitor<void> {
public:
    virtual ~Resolver() = default;
    Resolver(Interpreter& interpreter, Lox& lox);
    void resolve(const std::vector<std::unique_ptr<Stmt>>& statements);

    void visitBlock(const Block& stmt) override;
    void visitClass(const Class& stmt) override;
    void visitVarStmt(const VarStmt& stmt) override;
    void visitFunction(const Function& stmt) override;
    void visitExpression(const Expression& stmt) override;
    void visitIf(const If& stmt) override;
    void visitPrint(const Print& stmt) override;
    void visitWhile(const While& stmt) override;
    void visitReturn(const Return& stmt) override;
    void visitBreak(const Break& stmt) override;
    
    std::any visitVar(const Var& expr) override;
    std::any visitAssign(const Assign& expr) override;
    std::any visitBinary(const Binary& expr) override;
    std::any visitLambda(const Lambda& expr) override;
    std::any visitCall(const Call& expr) override;
    std::any visitGet(const Get& expr) override;
    std::any visitSet(const Set& expr) override;
    std::any visitGrouping(const Grouping& expr) override;
    std::any visitLiteral(const Literal& expr) override;
    std::any visitLogical(const Logical& expr) override;
    std::any visitUnary(const Unary& expr) override;
    std::any visitComma(const Comma& expr) override;
    std::any visitTernary(const Ternary& expr) override;

private:
    Interpreter& interpreter;
    Lox& lox;    
    enum class FunctionType {
        NONE,
        FUNCTION,
        METHOD
    };
    FunctionType currentFunction = FunctionType::NONE;
    void beginScope();
    void endScope();
    void resolve(const Stmt& stmt);
    void resolve(const Expr& expr);
    void resolveLocal(const Expr& expr, Token name);
    void resolveFunction(const Function& function, FunctionType type);
    void resolveLambda(const Lambda& function);
    void declare(Token name);
    void define(Token name);

    std::vector<std::unordered_map<std::string, bool>> scopes;

};