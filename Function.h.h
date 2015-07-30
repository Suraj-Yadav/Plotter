class UserFunc {
	char name[11];
	expression expr;
	char arg[20];
	unsigned no_of_parameter;
public:
	void set(const char given_name[], const char args[], const char defin[]) {
		char temp[101], argument[20], expr[200];
		strnchcpy(name, given_name, 10, ' ');
		strnchcpy(temp, defin, 100);
		strnchcpy(arg, args, 20);
		stripwhite(temp);
		stripwhite(arg);
		no_of_parameter = 0;
		/* char *next_argument=strnchcpy(argument,arg,20,',');
		 while(*next_argument!='\0'){
		 	no_of_parameter++;
					next_argument=strnchcpy(argument,next_argument+1,20,',');
		 };
		 if(argument[0])
					no_of_parameter++;*/
	}
	void print() {
		Show("Function Name", name);
//Show("Expression",expr);
		Show("No of args", no_of_parameter);
		Show("Args", arg);
	}
	double evaluate(const double act_par[]) {
		return -1;
	}
};
UserFunc Functions[10];
double func_helper(unsigned index, double args[]) {
	return Functions[index].evaluate(args);
}