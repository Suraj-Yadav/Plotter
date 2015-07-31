#ifndef FUNC_H
#define FUNC_H
#include <string>
#include <vector>
class FuncSig {
  public:
	FuncSig() {noOfArgs = 0;}
	FuncSig(const std::string &n, int A);
	bool operator<(const FuncSig &op) const;
	bool operator==(const FuncSig &op) const;
  protected:
  private:
	int noOfArgs; //!< Member variable "noOfArgs"
	std::string name; //!< Member variable "name"
};
#endif // FUNC_H
