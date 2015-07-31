#ifndef TERM_H
#define TERM_H
#include <string>
#include <vector>
enum TermType {
	NUM = 0, LBRA, RBRA, FUNC, CONST, LOCPAR, OPER, COMMA, ASG
};
struct Term {
	double val;
	int noOfArgs;
	std::string name;
	int index;
	static size_t count;
	std::vector<Term *> child;
	/** Default constructor */
	Term();
	/** Default destructor */
	~Term();
	TermType type;
	Term(const Term &v);
	Term &operator=(const Term &t);
	explicit Term(double);
	explicit Term(TermType t);
	Term(TermType t, std::string NAME);
	Term(TermType t, std::string NAME, int i);
	bool operator==(const Term &op) const;
	std::string toString();
};
#endif // TERM_H
