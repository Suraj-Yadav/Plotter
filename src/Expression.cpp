#include <iostream>
#include <map>
#include "Expression.h"
#include "FuncSig.h"
#define val(x) std::cout<<#x"="<<x<<"\n"
#ifdef __linux__
char leftUp[] = {'\t', 0};
char leftDown[] = "\xe2\x94\x82\t";
char rightUp[] = "\xe2\x94\x82\t";
char rightDown[] = {'\t', 0};
char Up[] = "\xe2\x94\x8c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80";//{ -38, -60, -60, -60, -60, -60, -60, -60, 0};
char Down[] = "\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80";//{ -64, -60, -60, -60, -60, -60, -60, -60, 0};
#elif _WIN32
char rightUp[] = { -77, '\t', 0};
char rightDown[] = {'\t', 0};
char mid[] = { -61, -60, -60, -60, -60, -60, -60, -60, 0};
char last[] = { -64, -60, -60, -60, -60, -60, -60, -60, 0};
#endif
std::map<FuncSig, Expression> UserDef;
Expression::Expression() {
	std::cout<<"Expression::Calling Default\n";
	terms = nullptr;
}
Expression::~Expression(){
	delete terms;
}
Expression::Expression(Term *t) {
	std::cout<<"Expression::Calling Term\n";
	terms = t;
}
Expression::Expression(std::string &str) {
	std::cout<<"Expression::Calling string\n";
	terms = nullptr;
	assign(str);
}
Expression::Expression(const Expression &L){
    terms = new Term(*L.terms);
}
Expression & Expression:: operator=(const Expression &L){
    if(this==&L)
		return *this;
	delete terms;
    terms = new Term(*L.terms);
    return *this;
}

bool Expression::createTree(const std::vector<Term *> &postfix) {
	delete terms;
	std::stack<Term *> Stack;
	for(Term *t : postfix) {
		if(t == nullptr)
			return false;
		switch(t->type) {
			case TermType::NUM:	{
					Stack.push(t);
					break;
				}
			case TermType::CONST:	{
					t->type = TermType::FUNC;
					t->index = isPreDef(t->name, 0);
					Stack.push(t);
					break;
				}
			case TermType::LOCPAR:	{
					Stack.push(t);
					break;
				}
			case TermType::OPER: {
					if(t->name.at(0) != '-' || t->index != 1) {
						Term *second = Stack.top();
						Stack.pop();
						Term *first = Stack.top();
						Stack.pop();
						t->child.push_back(first);
						t->child.push_back(second);
						Stack.push(t);
					}
					else {
						Term *first = Stack.top();
						Stack.pop();
						t->child.push_back(first);
						Stack.push(t);
					}
					break;
				}
			case TermType::FUNC: {
					std::vector<Term *> args;
					args.reserve(t->noOfArgs);
					for(int i = 0; i < t->noOfArgs; ++i) {
						args.push_back(Stack.top());
						Stack.pop();
					}
					std::reverse(args.begin(), args.end());
					t->child.swap(args);
					t->index = isPreDef(t->name, t->noOfArgs);
					Stack.push(t);
					break;
				}
			default:
				break;
		}
	}
	terms = Stack.top();
	Stack.pop();
	print();
	terms = simplify(terms);
	return true;
}
double Expression::evaluate(Term *x, const std::vector<double> &localVals, double xVal, double yVal) {
	if(x == nullptr)
		return 0.0;
	switch(x->type) {
		case TermType::NUM:
			return x->val;
		case TermType::FUNC:	{
				if(x->name.compare("x") == 0)
					return xVal;
				if(x->name.compare("y") == 0)
					return yVal;
				std::vector<double> argsVal;
				argsVal.reserve(x->noOfArgs);
				for(Term *r : x->child)
					argsVal.push_back(evaluate(r, localVals, xVal, yVal));
//				val(index);
				if(x->index != -1)
					return getPreDef(x->index, argsVal);
				auto find = UserDef.find(FuncSig(x->name, x->noOfArgs));
				if(find != UserDef.end())
					return find->second.evaluate(argsVal);
				return 0.0;
			}
		case TermType::OPER:	{
				if(x->name.at(0) != '-' || x->index != 1) {
					double first = evaluate(x->child[0], localVals, xVal, yVal);
					double second = evaluate(x->child[1], localVals, xVal, yVal);
					switch(x->name.at(0)) {
						case '%':
							return fmod(first, second);
						case '*':
							return first * second;
						case '+':
							return first + second;
						case '/':
							return first / second;
						case '-':
							return first - second;
						case '<':
							return first < second;
						case '>':
							return first > second;
						case '^':
							return pow(first, second);
						default:
							return 0;
					}
				}
				else {
					double first = evaluate(x->child[0], localVals, xVal, yVal);
					return -first;
				}
			}
		case TermType::LOCPAR:	{
				return localVals[x->index];
			}
		default:
			return 0;
	}
}
void Expression::print(Term *e, std::string str, bool l) {
	if(e == nullptr)
		return;
	std::cout << str << ((l) ? (last) : (mid)) << e->toString() <<","<<e;
//	std::cout << "," << e->child.size();
	//if (e->ret_type)
	//	std::cout << "," << e->ret_type->name << "\n";
	//else
	std::cout << "\n";
	if(e->child.size() > 1)
		for(size_t i = 0; i < e->child.size() - 1; ++i)
			print(e->child[i], str + ((l) ? (rightDown) : (rightUp)), false);
	if(e->child.size() > 0)
		print(e->child[e->child.size() - 1], str + ((l) ? (rightDown) : (rightUp)), true);
}
void Expression::print() {
	std::cout << "\n" << terms->name << "\n";
	if(terms->child.size() > 1)
		for(size_t i = 0; i < terms->child.size() - 1; ++i)
			print(terms->child[i], "", false);
	if(terms->child.size() > 0)
		print(terms->child[terms->child.size() - 1], "", true);
}
//operator	precedence	associativity
//	^			4			Right
//	*			3			Left
//	/			3			Left
//	%			3			Left
//	+			2			Left
//	-			2			Left
int getPrecedence(char ch, int i) {
	switch(ch) {
		case '^':
			return 4;
		case '*':
			return 3;
		case '%':
			return 3;
		case '/':
			return 3;
		case '+':
			return 2;
		case '-':
			return i ? 5 : 2;
		default:
			return 0;
	}
}
bool getAssociativity(char ch, int i) {
	switch(ch) {
		case '^':
			return true;
		case '*':
			return false;
		case '%':
			return false;
		case '/':
			return false;
		case '+':
			return false;
		case '-':
			return i ? true : false;
		default:
			return false;
	}
}
std::vector<Term *> getInfix(const std::vector<Token> &expr) {
	std::vector<Term *> infix;
	for(Token t : expr) {
		switch(t.tag) {
			case Tag::REAL:
				infix.push_back(new Term(std::stod(t.lexeme)));
				break;
			case Tag::COMM:
				infix.push_back(new Term(TermType::COMMA));
				break;
			case Tag::EQ:
				infix.push_back(new Term(TermType::ASG));
				break;
			case Tag::DIV:
				infix.push_back(new Term(TermType::OPER, "/"));
				break;
			case Tag::MUL:
				infix.push_back(new Term(TermType::OPER, "*"));
				break;
			case Tag::REM:
				infix.push_back(new Term(TermType::OPER, "%"));
				break;
			case Tag::POW:
				infix.push_back(new Term(TermType::OPER, "^"));
				break;
			case Tag::MINUS:	{
					if(infix.empty() ||
							infix.back()->type == TermType::LBRA ||
							infix.back()->type == TermType::COMMA ||
							infix.back()->type == TermType::OPER)
						infix.push_back(new Term(TermType::OPER, "-", 1));
					else
						infix.push_back(new Term(TermType::OPER, "-", 0));
					break;
				}
			case Tag::PLUS:
				infix.push_back(new Term(TermType::OPER, "+"));
				break;
			case Tag::RPAR:
				infix.push_back(new Term(TermType::RBRA));
				break;
			case Tag::LPAR: {
					if(!infix.empty() && infix.back()->type == TermType::CONST)
						infix.back()->type = TermType::FUNC;
					infix.push_back(new Term(TermType::LBRA));
					break;
				}
			case Tag::VAR:
				infix.push_back(new Term(TermType::CONST, t.lexeme));
				break;
			default:
				infix.push_back(new Term(TermType::FUNC, t.lexeme));
		}
	}
	return infix;
}
std::vector<Term *> getPostFix(const std::vector<Term *> &infix) {
	std::vector<Term *> postFix;
	std::stack<Term *> operStack;
	for(Term *t : infix) {
		switch(t->type) {
			case TermType::ASG:
				delete t;
				break;
			case TermType::NUM:
				postFix.push_back(t);
				break;
			case TermType::LOCPAR:
				postFix.push_back(t);
				break;
			case TermType::CONST:
				postFix.push_back(t);
				break;
			case TermType::FUNC:
				operStack.push(t);
				break;
			case TermType::COMMA:	{
					while(true) {
						if(operStack.empty())
							return std::vector<Term *>();
						if(operStack.top()->type == TermType::LBRA)
							break;
						postFix.push_back(operStack.top());
						operStack.pop();
					}
					operStack.top()->noOfArgs++;
					delete t;
					break;
				}
			case TermType::OPER: {
					while(true) {
						if(operStack.empty())
							break;
						if(!getAssociativity(t->name[0], t->index) &&
								getPrecedence(t->name[0], t->index) <= getPrecedence(operStack.top()->name[0], operStack.top()->index)) {
							postFix.push_back(operStack.top());
							operStack.pop();
						}
						else if(getAssociativity(t->name[0], t->index) &&
								getPrecedence(t->name[0], t->index) < getPrecedence(operStack.top()->name[0], operStack.top()->index)) {
							postFix.push_back(operStack.top());
							operStack.pop();
						}
						else
							break;
					}
					operStack.push(t);
					break;
				}
			case TermType::LBRA:
				operStack.push(t);
				break;
			case TermType::RBRA:	{
					while(true) {		//Poping till we get '('
						if(operStack.empty())
							return std::vector<Term *>();
						if(operStack.top()->type == TermType::LBRA)
							break;
						postFix.push_back(operStack.top());
						operStack.pop();
					}
					Term *temp = operStack.top();
					operStack.pop();	//pop the '('break;
					if(!operStack.empty() && operStack.top()->type == TermType::FUNC) {
						operStack.top()->noOfArgs = temp->noOfArgs + 1;
						postFix.push_back(operStack.top());
						operStack.pop();
					}
					delete t;
					delete temp;
					break;
				}
		}
	}
	while(true) {
		if(operStack.empty())break;
		if(operStack.top()->type == TermType::LBRA || operStack.top()->type == TermType::RBRA)
			return std::vector<Term *>();
		postFix.push_back(operStack.top());
		operStack.pop();
	}
	return postFix;
}
Term *Expression::simplify(Term *t) {
	switch(t->type) {
		case NUM:	return t;
		case LBRA:	return t;
		case RBRA:	return t;
		case FUNC: {
				bool cond = t->child.size() > 0;
				for(size_t i = 0; i < t->child.size(); ++i) {
					t->child.at(i) = simplify(t->child.at(i));
					if(t->child.at(i)->type != TermType::NUM)
						cond = false;
				}
				if(cond) {
					Term *temp = new Term(evaluate(t, std::vector<double>(), 0, 0));
					delete t;
					return temp;
				}
				return t;
			}
		case CONST:	return t;
		case LOCPAR:	return t;
		case OPER: {
				if(t->name.at(0) != '-' || t->index != 1) {
					t->child[0] = simplify(t->child[0]);
					t->child[1] = simplify(t->child[1]);
					/*switch (t->name.at(0)) {
						case '%':	return t;
						case '*':	return t;
						case '+':	return t;
						case '/':	return t;
						case '-':	return t;
						case '<':	return t;
						case '>':	return t;
						case '^':	return t;
					}*/
//                    std::cout<<t->toString()<<"'s Children"<<t->child[0]->toString()<<","<<t->child[1]->toString()<<"\n";
					if(t->child[0]->type == TermType::NUM && t->child[1]->type == TermType::NUM) {
						Term *temp = new Term(evaluate(t, std::vector<double>(), 0, 0));
						delete t;
						return temp;
					}
				}
				else {
					t->child[0] = simplify(t->child[0]);
					Term *x = t->child[0];
					if(x->name.at(0) != '-' || x->index != 1) {
						switch(x->name.at(0)) {
							case '%':
								break;
							case '*':
								break;
							case '+':
								break;
							case '/':
								break;
							case '-':
								std::reverse(x->child.begin(), x->child.end());
								delete t;
								return x;
								break;
							case '<':
								break;
							case '>':
								break;
							case '^':
								break;
						}
					}
					else {
						Term *ret = x->child[0];
						delete x;
						delete t;
						return ret;
					}
				}
			}
			break;
		case COMMA:
			break;
		case ASG:
			break;
	}
	return t;
}
int isFuncDef(const std::vector<Term *> &expr) {
	if(expr.size() < 3)
		return -1;
	if(expr.at(0)->type == TermType::CONST && expr.at(1)->type == TermType::ASG)
		return 1;
	if(expr.at(0)->type == TermType::FUNC && expr.at(1)->type == TermType::LBRA) {
		size_t i = 2;
		int bracket = 0;
		while(expr.at(i)->type != TermType::RBRA && !bracket) {
			if(expr.at(i)->type == TermType::LBRA)
				++bracket;
			else if(expr.at(i)->type == TermType::RBRA)
				bracket--;
			++i;
			if(i == expr.size())
				return -1;
		}
		if(i + 1 == expr.size())
			return -1;
		if(expr.at(i + 1)->type == TermType::ASG)
			return i + 1;
		else
			return -1;
	}
	return -1;
}
std::vector<Term *> getArgs(const std::vector<Term *> &expr, int index) {
	std::vector<Term *> args;
	for(int i = 2; i < index; i++)
		if(expr.at(i)->type == TermType::CONST)
			args.push_back(expr.at(i));
	return args;
}
int getIndex(const std::vector<Term *> &expr, const std::string &name) {
	int index = 0;
	for(Term *t : expr) {
		if(t != nullptr && t->type == TermType::CONST && t->name.compare(name) == 0)
			return index;
		index++;
	}
	return -1;
}
bool addFunc(const std::vector<Term *> &expr, int index) {
	std::string name = expr.at(0)->name;
	std::vector<Term *> def(expr.begin() + 1 + index, expr.end());
	std::cout << name << "\n";
	std::vector<Term *> args = getArgs(expr, index);
	for(Term *t : args) {
		std::cout << t->toString() << ",";
	}
	std::cout << "\n";
	for(Term *t : def) {
		std::cout << t->toString() << ",";
	}
	std::cout << "\n";
	for(Term *t : def) {
		if(t->type == TermType::CONST) {
			int index = getIndex(args, t->name);
			if(index > -1) {
				t->type = TermType::LOCPAR;
				t->index = index;
			}
		}
	}
	for(Term *t : def) {
		std::cout << t->toString() << ",";
	}
	std::cout << "\n";
	for(int i = 0; i <= index; i++)
		delete expr[i];
	std::vector<Term *> postfix = getPostFix(def);
	FuncSig f(name, args.size());
	auto p = UserDef.emplace(f, Expression());
	if(!p.second)
		std::cout << "Not Done\n";
	else
		UserDef[f].createTree(postfix);
	return true;
}
void Expression::assign(const std::string &str) {
	std::vector<Token> expr = getTokens(str);
	std::vector<Term *> infix = getInfix(expr);
	//for (Term *t : infix) {std::cout << t->toString() << ",";}
	//std::cout << "\n";
	//int index = isFunc(infix);
	//if (index == -1) {
	std::vector<Term *> postfix = getPostFix(infix);
//	postfix[0]->type = TermType::LOCPAR;
//	postfix[0]->index = 0;
	//		for (Term *t : postfix) {std::cout << t->toString() << ",";}
	//		std::cout << "\n";
	//		std::vector<double> x;
	//		x.push_back(12);
	createTree(postfix);
}
