#include <string>
#include <vector>
#include <cmath>
int isPreDef(const std::string &name, int args) {
	if(name.compare("sin") == 0 && args == 1)	return 1;
	if(name.compare("cos") == 0 && args == 1)	return 2;
	if(name.compare("tan") == 0 && args == 1)	return 3;
	if(name.compare("asin") == 0 && args == 1)	return 4;
	if(name.compare("acos") == 0 && args == 1)	return 5;
	if(name.compare("atan") == 0 && args == 1)	return 6;
	if(name.compare("atan") == 0 && args == 2)	return 7;
	if(name.compare("sinh") == 0 && args == 1)	return 8;
	if(name.compare("cosh") == 0 && args == 1)	return 9;
	if(name.compare("tanh") == 0 && args == 1)	return 10;
	if(name.compare("asinh") == 0 && args == 1)	return 11;
	if(name.compare("acosh") == 0 && args == 1)	return 12;
	if(name.compare("atanh") == 0 && args == 1)	return 13;
	if(name.compare("ceil") == 0 && args == 1)	return 14;
	if(name.compare("floor") == 0 && args == 1)	return 15;
	if(name.compare("abs") == 0 && args == 1)	return 16;
	if(name.compare("log") == 0 && args == 1)	return 17;
	if(name.compare("log") == 0 && args == 2)	return 18;
	if(name.compare("ln") == 0 && args == 1)	return 19;
	if(name.compare("sqrt") == 0 && args == 1)	return 20;
	if(name.compare("pi") == 0 && args == 0)	return 21;
	if(name.compare("e") == 0 && args == 0)	return 22;
	return -1;
}
double getPreDef(int which, const std::vector<double> &args) {
	switch(which) {
		case 1:
			return sin(args[0]);
		case 2:
			return cos(args[0]);
		case 3:
			return tan(args[0]);
		case 4:
			return asin(args[0]);
		case 5:
			return acos(args[0]);
		case 6:
			return atan(args[0]);
		case 7:
			return atan2(args[0], args[1]);
		case 8:
			return sinh(args[0]);
		case 9:
			return cosh(args[0]);
		case 10:
			return tanh(args[0]);
		case 11:
			return asinh(args[0]);
		case 12:
			return acosh(args[0]);
		case 13:
			return atanh(args[0]);
		case 14:
			return ceil(args[0]);
		case 15:
			return floor(args[0]);
		case 16:
			return fabs(args[0]);
		case 17:
			return log10(args[0]);
		case 18:
			return log(args[0]) / log(args[1]);
		case 19:
			return log(args[0]);
		case 20:
			return sqrt(args[0]);
		case 21:
			return 3.14159265358979323846;
		case 22:
			return 2.7182818284590452354;
		default:
			return 0;
	}
}

