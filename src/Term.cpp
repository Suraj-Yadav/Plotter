#include "Term.h"
#include <cmath>
#include <iostream>
char TermName[][17] = {
	"NUM", "LBRA", "RBRA", "FUNC", "CONST", "LOCPAR", "OPER", "COMMA", "ASG"
};
size_t Term::count = 0;
Term::Term() {
	type = NUM;
	val = 0.0;
	noOfArgs = 0;
	index = 0;
	count++;
}
Term::~Term() {
	count--;
	std::cout << "count=" << count << "\t";
//    std::cout<<"Deleting"<<this<<"\n";
	for(Term *t : child)
		delete t;
}
Term::Term(const Term &v) {
	count++;
	type = v.type;
	val = v.val;
	noOfArgs = v.noOfArgs;
	name = v.name;
	index = v.index;
    for(Term *t:v.child)
        this->child.emplace_back(new Term(*t));
}
Term & Term::operator=(const Term &T) {
  // check for "self assignment" and do nothing in that case
  if (this == &T) return *this;
  else {
    index = T.index;
    name = T.name;
    noOfArgs = T.noOfArgs;
    type = T.type;
	for(Term *t : child)
		delete t;
	child.clear();
	for(Term *t : T.child)
		child.push_back(new Term(*t));
  }
	return *this;                   // return this Term
}
Term::Term(double v) {
	count++;
	type = NUM;
	val = v;
	noOfArgs = 0;
	index = 0;
	name = std::to_string(v);
}
Term::Term(TermType t) {
	count++;
	type = t;
	noOfArgs = 0;
	index = 0;
	val = 0;
}
Term::Term(TermType t, std::string NAME) {
	count++;
	type = t;
	name = NAME;
	noOfArgs = 0;
	index = 0;
	val = 0;
}
Term::Term(TermType t, std::string NAME, int i) {
	count++;
	type = t;
	name = NAME;
	noOfArgs = 0;
	index = i;
	val = 0;
}
std::string Term::toString() {
	std::string str = name;
	str.append(",");
	str.append(TermName[type]);
	//if(noOfArgs)
	{
		str.append(",");
		str.append(std::to_string(index));
	}
	if(name[0] != '<' && name[0] != '>')
		return "<" + str + ">";
	else
		return "[" + str + "]";
}
bool Term::operator==(const Term &op) const {
	if(type != op.type)
		return false;
	switch(type) {
		case NUM:
			return this->val == op.val;
		case FUNC:
		case CONST:
		case OPER:
			return (op.name.compare(this->name) == 0) && (this->index == op.index);
		case LOCPAR:
			return (this->index == op.index);
		case LBRA:
		case RBRA:
		case COMMA:
		case ASG:
			return true;
	}
	return false;
}
