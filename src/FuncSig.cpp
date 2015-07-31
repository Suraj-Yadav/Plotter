#include "FuncSig.h"
FuncSig::FuncSig(const std::string &n, int A): noOfArgs(A), name(n)  {}
bool FuncSig::operator<(const FuncSig &op) const {
	if(name.compare(op.name) < 0) 	return true;
	if(name.compare(op.name) > 0)	return false;
	if(noOfArgs < op.noOfArgs)	return true;
	return false;
}
bool FuncSig::operator==(const FuncSig &op) const {
	if(name.compare(op.name) == 0 && noOfArgs < op.noOfArgs)	return true;
	return false;
}
