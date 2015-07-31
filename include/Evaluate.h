const char allowed_characters[] = " %()*+,-./0123456789<=>ABCDEFGHIJKLMNOPQRSTUVWXYZ^_abcdefghijklmnopqrstuvwxyz";
// 00000000001111111111222222222233333333334444444444555555555566666666667777777
// 01234567890123456789012345678901234567890123456789012345678901234567890123456
enum char_types { OPER, LPAREN, RPAREN, DOT, COMMA, NUM, ALPHA, ERR };
bool is_fine_to_break(char_types curr, char_types next) {
	if ((curr == NUM && next == DOT) || (next == NUM && curr == DOT))
		return false;
	if (curr == ALPHA && next == NUM)
		return false;
	if (curr == ALPHA && next == ALPHA)
		return false;
	if (curr == NUM && next == NUM)
		return false;
	return true;
}
int whichchar(char ch) {
	int start = 0, end = strlen(allowed_characters);
	while (start <= end) {
		int mid = (start + end) / 2;
		if (allowed_characters[mid] == ch) return mid;
		if (allowed_characters[mid] < ch) start = mid + 1;
		else end = mid - 1;
	}
	return -1;
}
char_types which_char_types(char ch) {
	if (ch == '(' || ch == '[' || ch == '&' || ch == '{') return LPAREN;
	if (ch == ')' || ch == ']' || ch == '|' || ch == '}') return RPAREN;
	if (ch == '%' || ch == '*' || ch == '+' || ch == '-' || ch == '/' || ch == '<' || ch == '=' || ch == '>' || ch == '^')
		return OPER;
	if (ch == '.') return DOT;
	if (ch == ',') return COMMA;
	if (ch <= '9' && ch >= '0') return NUM;
	if ((ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a') || ch == '_') return ALPHA;
	return ERR;
}
/*bool bracket_check(char expr[]){
 int count=0;
 for(int i=0;expr[i]!='\0';i++)
 if(expr[i]=='|')
 count++;
 if(count%2==1){
 std::cout<<"error: no closing bracket found for \'|\'.\n";
 return false;
 }
 for(int i=0;count>0;i++)
 if(expr[i]=='|'){
 expr[i]='$';
 count--;
 }
 char options[4][3]={"()","{}","[]","$|"};
 int bracket_stack[20],bracket_top=0;
 for(int i=0;expr[i]!='\0';i++){
 if(expr[i]=='(')
 bracket_stack[bracket_top++]=0;
 else if(expr[i]=='{')
 bracket_stack[bracket_top++]=1;
 else if(expr[i]=='[')
 bracket_stack[bracket_top++]=2;
 else if(expr[i]=='[')
 bracket_stack[bracket_top++]=3;
 else if(expr[i]==')'){
 if(bracket_top==0){
 std::cerr<<"error: no opening bracket for \')\'.\n";
 return false;
 }
 else if(bracket_stack[bracket_top-1]!=0){
 std::cerr<<"error: expected \'"<<options[bracket_stack[bracket_top-1]][1]<<"\' Found:- \')\'\n";
 return false;
 }
 else
 bracket_top--;
 }
 else if(expr[i]==']'){
 if(bracket_top==0){
 std::cerr<<"error: no opening bracket for \']\'.\n";
 return false;
 }
 else if(bracket_stack[bracket_top-1]!=2){
 std::cerr<<"error: expected \'"<<options[bracket_stack[bracket_top-1]][1]<<"\' Found:- \']\'\n";
 return false;
 }
 else
 bracket_top--;
 }
 else if(expr[i]=='}'){
 if(bracket_top==0){
 std::cerr<<"error: no opening bracket for \'}\'.\n";
 return false;
 }
 else if(bracket_stack[bracket_top-1]!=1){
 std::cerr<<"error: expected \'"<<options[bracket_stack[bracket_top-1]][1]<<"\' Found:- \'}\'\n";
 return false;
 }
 else
 bracket_top--;
 }
 else if(expr[i]=='|'){
 if(bracket_top==0){
 std::cerr<<"error: no opening bracket for \'}\'.\n";
 return false;
 }
 else if(bracket_stack[bracket_top-1]!=3){
 std::cerr<<"error: expected \'"<<options[bracket_stack[bracket_top-1]][1]<<"\' Found:- \'|\'\n";
 return false;
 }
 else
 bracket_top--;
 }
 }
 if(bracket_top>0){
 std::cerr<<"error: no closing bracket for \'"<<options[bracket_stack[bracket_top-1]][0]<<"\' found.\n";
 }
 return true;
}*/
bool create_tokens(const char expr[], char token_queue[]) {
	char_types current, next;
	char copy[200], temp[50];
	int j = 0;
	for (int i = 0; expr[i] != '\0'; i++) {
// if(whichchar(expr[i])<0)
// return false;
		copy[j++] = expr[i];
	}
	copy[j] = '\0';
	j = 0;
	bool func_mode = false;
	unsigned paren_count = 0;
	std::strcpy(token_queue, "(#");
	current = which_char_types(copy[0]);
	temp[j++] = copy[0];
	for (int i = 1; copy[i] != '\0'; i++) {
		next = which_char_types(copy[i]);
		if (current == ALPHA && next == LPAREN) {
			func_mode = true;
		}
		if (func_mode && next == LPAREN)
			paren_count++;
		if (func_mode && next == RPAREN) {
			paren_count--;
			if (paren_count == 0) {
				temp[j++] = copy[i];
				func_mode = false;
				continue;
			}
		} else if (current == ERR && copy[i - 1] == '$') {
			temp[j++] = copy[i];
			current = NUM;
			continue;
		}
		if (!func_mode && copy[i] == '-' && (current == RPAREN || current == NUM || current == ALPHA) && (which_char_types(copy[i + 1]) == LPAREN || which_char_types(copy[i + 1]) == NUM || which_char_types(copy[i + 1]) == ALPHA || which_char_types(copy[i + 1]) == OPER)) {
			temp[j++] = '#';
			temp[j++] = '+';
			temp[j++] = '\0';
			strcat(token_queue, temp);
			strcat(token_queue, "#");
			j = 0;
		} else if (!func_mode && is_fine_to_break(current, next)) {
			temp[j++] = '\0';
			strcat(token_queue, temp);
			strcat(token_queue, "#");
			j = 0;
		}
		temp[j++] = copy[i];
		current = next;
	}
	temp[j] = '\0';
	strcat(token_queue, temp);
	strcat(token_queue, "#)");
	return true;
}
//`pkg-config --libs gtk+-3.0`
//`pkg-config --cflags gtk+-3.0`
/*bool get_next_token(char token_list[], int &pos, char dest[]){
 if(token_list[pos]=='\0')
 return false;
 int end=-1,i,j;
 for(end=pos+1;token_list[end]!='#'&&token_list[end]!='\0';end++);
 for(i=0,j=pos;j<end;i++,j++)
 dest[i]=token_list[j];
 dest[i]='\0';
 if(token_list[end]=='\0')
 pos=end;
 else
 pos=end+1;
 return true;
}*/
//double evaluate(char expression[300]){
// char token_queue[500]="(#";
// if(bracket_check(expression)&&create_token(expression,token_queue)){
// std::cout<<token_queue<<"\n";
// char temp[30];
// func ops[6], current;
// double vals[6];
// int ops_top=0,vals_top=0,pos=0;
// while (get_next_token(token_queue,pos,temp)) {
// if (get_ops(temp)!=WRO) {
//				current=func(get_ops(temp));
//				while(current.get_precedence()>ops[ops_top-1].get_precedence()){
// int no_of_vals_req=ops[ops_top-1].get_no_of_operand();
// double args[no_of_vals_req];
// for(int i=0;i<no_of_vals_req;i++)
//						args[no_of_vals_req-1-i]=vals[--vals_top];
//					if(ops[ops_top-1].get_func_type()==SUPP)
//						vals[vals_top++]=value(ops[ops_top-1].get_supp_func(),args[0],args[1]);
// ops_top--;
// }
// if(current.get_func_type()==SUPP&&current.get_supp_func()==RPAR)
// ops_top--;
// }
// //this is where code for user defined function should be inserted
// else {vals[vals_top++]=to_double(temp);}
// }
// return vals[0];
// }
// return 0.0;
//}
int main2() {
	char expression[100] = "(2+3)";
//while(gets(expression))
//int test[3]={1,2,3};
// std::cout<<evaluate(expression)<<"\n";
	return 0;
}