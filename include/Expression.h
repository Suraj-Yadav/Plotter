#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <vector>
#include <stack>
#include <string>
#include <algorithm>


#include "Term.h"
#include "Token.h"
class Expression {
  public:
	Expression();
	explicit Expression(Term *t);
	explicit Expression(std::string &str);
	~Expression();
    Expression(const Expression &L);             // copy constructor
    Expression & operator=(const Expression &L); // assignment
	/** Access valid
	 * \return The current value of valid
	 */
	void print();
	void assign(const std::string &expr);
	double evaluate(double x = 0, double y = 0) {return evaluate(terms, std::vector<double>(), x, y);}
	double evaluate(const std::vector<double> &localVals = std::vector<double>(), double x = 0, double y = 0) {return evaluate(terms, localVals, x, y);}
	bool createTree(const std::vector<Term *> &postfix);
	static void print(Term *e, std::string str, bool l);
  protected:
  private:
	Term *terms; //!< Member variable "*terms"
	Term *simplify(Term *t);
	double evaluate(Term *t, const std::vector<double> &localVals, double x = 0, double y = 0);
};
std::vector<Term *> getInfix(const std::vector<Token> &expr);
std::vector<Term *> getPostFix(const std::vector<Term *> &infix);
int isFuncDef(const std::vector<Term *> &expr);
bool addFunc(const std::vector<Term *> &expr, int index);
int isPreDef(const std::string &name, int args);
double getPreDef(int which, const std::vector<double> &args);
#endif // EXPRESSION_H
/*
std::string str = "(12.1-2)*8";
	while (std::getline(std::cin, str)) {
		std::vector<Token> expr = getTokens(str);
		for (auto t : expr) {
			std::cout << t.toString() << ",";
		}
		std::cout << "\n";
		std::vector<Term> infix = getInfix(expr);
		for (Term t : infix) {
			std::cout << t.toString() << ",";
		}
		std::cout << "\n";
		int index = isFunc(infix);
		if (index == -1) {
			std::vector<Term> postfix = getPostFix(infix);
			for (Term t : postfix) {
				std::cout << t.toString() << ",";
			}
			std::cout << "\n";
			Expression a;
			a.createTree(postfix);
			a.print();
			std::cout << a.evaluate() << "\n";
		}
		else {
			std::cout << index << "\n";
			addFunc(infix, index);
		}
	}
*/

