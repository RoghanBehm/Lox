#include <string>
#include "Expr/ExprVisitor.hpp"
#include "Expr/Expr.hpp"
#include "Expr/Binary.hpp"
#include "Expr/Grouping.hpp"
#include "Expr/Literal.hpp"
#include "Expr/Unary.hpp"
#include "Expr/Comma.hpp"
#include "Expr/Ternary.hpp"
#include "to_string.hpp"
#include "token_type.hpp"

class AstPrinter : public ExprVisitor<std::any> {
    public:
        std::string print(const Expr& expr) {
            return std::any_cast<std::string>(expr.accept(*this));
        }
        std::any visitBinary(const Binary& expr) override {
            return parenthesize(expr.getOp().lexeme, expr.getLeftExpr(), expr.getRightExpr());
        }
        std::any visitGrouping(const Grouping& expr) override {
            return parenthesize("group", expr.getExpr());
        }
        std::any visitLiteral(const Literal& expr) override {
            if (!expr.getLiteral().has_value()) return "nil";
            return anyToString(expr.getLiteral());
        }
        std::any visitUnary(const Unary& expr) override {
            return parenthesize(expr.getOp().lexeme, expr.getRightExpr());
        }
        std::any visitAssign(const Assign& expr) override {
            return {};
        }

        std::any visitCall(const Call& expr) override {
            return {};
        }

        std::any visitLogical(const Logical& expr) override {
            return {};
        }

        std::any visitVar(const Var& expr) override {
            return {};
        }

        std::any visitComma(const Comma& expr) override {
            return {};
        }

        std::any visitTernary(const Ternary& expr) override {
            return {};
        }


        
    private:
        std::string parenthesize(const std::string& name, const Expr& a) {
            return "(" + name + " " + print(a) + ")";
        }

        std::string parenthesize(const std::string& name, const Expr& a, const Expr& b) {
            return "(" + name + " " + print(a) + " " + print(b) + ")";
        }

        std::string rpn(const std::string& name, const Expr& a) {
            return print(a) + " " + name;
        }

        std::string rpn(const std::string& name, const Expr& a, const Expr& b) {
            return print(a) + " " + print(b) + " " + name;
        }
};


int printer() {
    using std::make_unique;

    // -123
    std::unique_ptr<Expr> unaryExpr = make_unique<Unary>(
        Token(TokenType::MINUS, "-", std::any(), 1),
        make_unique<Literal>(123.0)
    );

    // (45.67)
    std::unique_ptr<Expr> groupingExpr = make_unique<Grouping>(
        make_unique<Literal>(45.67)
    );

    // -123 * (45.67)
    std::unique_ptr<Expr> expression = make_unique<Binary>(
        std::move(unaryExpr),
        Token(TokenType::STAR, "*", std::any(), 1),
        std::move(groupingExpr)
    );

  //  AstPrinter printer;
  //  std::string result = printer.print(*expression);
   // std::cout << result << std::endl;

    return 0;

}