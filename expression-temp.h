unsigned find_func(const char name[]) {
	return 0;
}
bool check_for_num(const char data[]) {
	const char *s = data;
	if (*s == '.' && *(s + 1) == '\0')
		return false;
	while (*s != '.' && *s != '\0')
		if (*s > '9' || *s < '0')
			return false;
		else
			s++;
	if (*s == '.')
		s++;
	while (*s != '.' && *s != '\0')
		if (*s > '9' || *s < '0')
			return false;
		else
			s++;
	return true;
}
bool check_for_func(const char data[], char funcname[11], char arg[]) {
	char *pos = strnchcpy(funcname, data, 10, '(');
	if (*pos == '\0') {
		strcpy(arg, "");
		return true;
	}
	if (*pos != '(')
		return false;
	pos = strnchcpy(arg, pos + 1, 30, ')', true);
	if (*pos != ')')
		return false;
	return true;
}
struct term {
	enum {
		NUM, LBRA, RBRA, FUNC, LOCPAR, OPER
	} type;
	union {
		double value;
		char symbol;
		unsigned index;
	} what;
	char args[50];
	term() {
		type = NUM;
		what.value = 0;
	}
};
class expression {
	term terms[20];
	unsigned int no_of_terms;
	bool valid;
public:
	bool set(const char expr[]) {
		no_of_terms = 0;
		char temp[300], temp_name[11], token_expr[300];
		create_tokens(expr, token_expr);
		char *next_argument = strnchcpy(temp, token_expr, 300, '#');
		while (*next_argument != '\0') {
			if (check_for_num(temp)) {
				terms[no_of_terms].type = term::NUM;
				terms[no_of_terms].what.value = std::atof(temp);
			} else if (temp[0] == '(')
				terms[no_of_terms].type = term::LBRA;
			else if (temp[0] == ')')
				terms[no_of_terms].type = term::RBRA;
			else if (temp[0] == '%' || temp[0] == '*' || temp[0] == '+' || temp[0] == '-' || temp[0] == '/' || temp[0] == '<' || temp[0] == '=' || temp[0] == '>' || temp[0] == '^') {
				terms[no_of_terms].type = term::OPER;
				terms[no_of_terms].what.symbol = temp[0];
			} else if (temp[0] == '$') {
				terms[no_of_terms].type = term::LOCPAR;
				terms[no_of_terms].what.index = temp[1];
			} else if (check_for_func(temp, temp_name, terms[no_of_terms].args)) {
				terms[no_of_terms].type = term::FUNC;
				terms[no_of_terms].what.index = find_func(temp_name);
			}
			no_of_terms++;
			next_argument = strnchcpy(temp, next_argument + 1, 300, '#');
		}
		if (temp[0]) {
			if (check_for_num(temp)) {
				terms[no_of_terms].type = term::NUM;
				terms[no_of_terms].what.value = std::atof(temp);
			} else if (temp[0] == '(')
				terms[no_of_terms].type = term::LBRA;
			else if (temp[0] == ')')
				terms[no_of_terms].type = term::RBRA;
			else if (temp[0] == '%' || temp[0] == '*' || temp[0] == '+' || temp[0] == '-' || temp[0] == '/' || temp[0] == '<' || temp[0] == '=' || temp[0] == '>' || temp[0] == '^') {
				terms[no_of_terms].type = term::OPER;
				terms[no_of_terms].what.symbol = temp[0];
			} else if (temp[0] == '$') {
				terms[no_of_terms].type = term::LOCPAR;
				terms[no_of_terms].what.index = temp[1];
			} else if (check_for_func(temp, temp_name, terms[no_of_terms].args)) {
				terms[no_of_terms].type = term::FUNC;
				terms[no_of_terms].what.index = find_func(temp_name);
			}
			no_of_terms++;
		}
		valid = validity_check();
		return valid;
	}
	bool isvalid() {
		return valid;
	}
	bool validity_check() {
		unsigned bracket_top = 0;
		for (unsigned i = 0; i < no_of_terms; i++) {
			if (terms[i].type == term::LBRA)
				bracket_top++;
			else if (terms[i].type == term::RBRA)
				if (terms[i - 1].type == term::OPER)
					return false;
				else
					bracket_top--;
			else if (terms[i].type == term::OPER && terms[i].what.symbol != '-')
				if (terms[i - 1].type == term::LBRA || terms[i - 1].type == term::OPER)
					return false;
		}
		if (bracket_top == 0)
			return true;
		else
			return false;
	}
	void print() {
		for (size_t i = 0; i < no_of_terms; i++) {
			std::cout << "<";
			switch (terms[i].type) {
				case term::NUM: std::cout << terms[i].what.value; break;
				case term::LBRA:	std::cout << "("; break;
				case term::RBRA:	std::cout << ")"; break;
				case term::FUNC:	std::cout << terms[i].what.index << ":" << terms[i].args; break;
				case term::LOCPAR:	std::cout << terms[i].what.index; break;
				case term::OPER:	std::cout << terms[i].what.symbol; break;
			}
			std::cout << ">,";
		}
		std::cout << "\b.\n";
	}
	double evaluate() {
		term temp;
		double val_stack[20];
		supported_ops ops_stack[20], current;
		unsigned val_top = 0, ops_top = 0;
		for (unsigned i = 0; i < no_of_terms; i++) {
			temp = terms[i];
			if (temp.type == term::LBRA) {
				ops_stack[ops_top++] = LPAR;
			} else if (temp.type == term::OPER) {
				current = get_ops(temp.what.symbol);
				while (get_precedence(current) < get_precedence(ops_stack[ops_top - 1])) {
					double a, b;
					a = val_stack[--val_top];
					b = val_stack[--val_top];
					switch (ops_stack[ops_top - 1]) {
						case ADD: val_stack[val_top++] = b + a; break;
//							case SUB: val_stack[val_top++]=b-a;break;
						case NEG: val_stack[val_top++] = a; val_stack[val_top++] = -b; break;
						case MUL: val_stack[val_top++] = b * a; break;
						case POW: val_stack[val_top++] = pow(b, a); break;
						case MOD: val_stack[val_top++] = fmod(b, a); break;
						case DIV: val_stack[val_top++] = b / a; break;
						default:	break;
					}
					ops_top--;
				}
				ops_stack[ops_top++] = current;
			} else if (temp.type == term::RBRA) {
				while (ops_stack[ops_top - 1] != LPAR) {
					double a, b;
					a = val_stack[--val_top];
					b = val_stack[--val_top];
					switch (ops_stack[ops_top - 1]) {
						case ADD: val_stack[val_top++] = b + a; break;
//							case SUB: val_stack[val_top++]=b-a;break;
						case NEG: val_stack[val_top++] = a; val_stack[val_top++] = -b; break;
						case MUL: val_stack[val_top++] = b * a; break;
						case POW: val_stack[val_top++] = pow(b, a); break;
						case MOD: val_stack[val_top++] = fmod(b, a); break;
						case DIV: val_stack[val_top++] = b / a; break;
						default:	break;
					}
					ops_top--;
				}
				ops_top--;
			} else if (temp.type == term::FUNC) {
				double arg_value[20];
				int i = 0;
				char args_temp[300];
				char *next_argument = strnchcpy(args_temp, temp.args, 300, ',');
				while (*next_argument != '\0') {
					expression temp_value;
					temp_value.set(args_temp);
					arg_value[i++] = temp_value.evaluate();
					next_argument = strnchcpy(args_temp, next_argument + 1, 300, ',');
				}
				if (args_temp[0]) {
					expression temp_value;
					temp_value.set(args_temp);
					arg_value[i++] = temp_value.evaluate();
				}
				val_stack[val_top++] = func_helper(temp.what.index, arg_value);
			} else if (temp.type == term::NUM) {
				val_stack[val_top++] = temp.what.value;
			}
		}
		return val_stack[0];
	}
	double evaluate(double local_par[]) {
		term temp;
		double val_stack[20];
		supported_ops ops_stack[20], current;
		unsigned val_top = 0, ops_top = 0;
		for (unsigned i = 0; i < no_of_terms; i++) {
			temp = terms[i];
			if (temp.type == term::LBRA) {
				ops_stack[ops_top++] = LPAR;
			} else if (temp.type == term::OPER) {
				current = get_ops(temp.what.symbol);
				while (get_precedence(current) < get_precedence(ops_stack[ops_top - 1])) {
					double a, b;
					a = val_stack[--val_top];
					b = val_stack[--val_top];
					switch (ops_stack[ops_top - 1]) {
						case ADD: val_stack[val_top++] = b + a; break;
//							case SUB: val_stack[val_top++]=b-a;break;
						case NEG: val_stack[val_top++] = a; val_stack[val_top++] = -b; break;
						case MUL: val_stack[val_top++] = b * a; break;
						case POW: val_stack[val_top++] = pow(b, a); break;
						case MOD: val_stack[val_top++] = fmod(b, a); break;
						case DIV: val_stack[val_top++] = b / a; break;
						default:	break;
					}
					ops_top--;
				}
				ops_stack[ops_top++] = current;
			} else if (temp.type == term::RBRA) {
				while (ops_stack[ops_top - 1] != LPAR) {
					double a, b;
					a = val_stack[--val_top];
					b = val_stack[--val_top];
					switch (ops_stack[ops_top - 1]) {
						case ADD: val_stack[val_top++] = b + a; break;
//							case SUB: val_stack[val_top++]=b-a;break;
						case NEG: val_stack[val_top++] = a; val_stack[val_top++] = -b; break;
						case MUL: val_stack[val_top++] = b * a; break;
						case POW: val_stack[val_top++] = pow(b, a); break;
						case MOD: val_stack[val_top++] = fmod(b, a); break;
						case DIV: val_stack[val_top++] = b / a; break;
						default:	break;
					}
					ops_top--;
				}
				ops_top--;
			} else if (temp.type == term::FUNC) {
				double arg_value[20];
				int i = 0;
				char args_temp[300];
				char *next_argument = strnchcpy(args_temp, temp.args, 300, ',');
				while (*next_argument != '\0') {
					expression temp_value;
					temp_value.set(args_temp);
					arg_value[i++] = temp_value.evaluate();
					next_argument = strnchcpy(args_temp, next_argument + 1, 300, ',');
				}
				if (args_temp[0]) {
					expression temp_value;
					temp_value.set(args_temp);
					arg_value[i++] = temp_value.evaluate();
				}
				val_stack[val_top++] = func_helper(temp.what.index, arg_value);
			} else if (temp.type == term::LOCPAR) {
				val_stack[val_top++] = local_par[temp.what.index];
			} else if (temp.type == term::NUM) {
				val_stack[val_top++] = temp.what.value;
			}
		}
		return val_stack[0];
	}
};