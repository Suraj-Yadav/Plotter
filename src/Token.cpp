#include "Token.h"
char TagName[][17] = {
	"SOPER",
	"SRES", "INCR", "DECR", "LPAR", "RPAR", "LBLK", "RBLK", "LSQB", "RSQB", "DOT", "ARROW", "PLUS", "MINUS", "LNOT", "BNOT", "ADDR", "MUL", "DIV", "REM", "BLS", "BRS", "LES", "ELES", "GRE", "EGRE", "EQ", "NEQ", "BAND", "BXOR", "BOR", "LAND", "LOR", "QUES", "COL", "ASSG", "COMM", "HASH", "SCOL", "POW",
	"EOPER",
	"STRING", "REAL", "VAR", "FUN", "REST"
};
Token::Token(std::string s, Tag tage) {
	tag = tage;
	lexeme = s;
}
std::string Token::toString() {
	std::string str = lexeme;
	str.append(",");
	str.append(TagName[tag]);
	if(lexeme[0] != '<' && lexeme[0] != '>')
		return "<" + str + ">";
	else
		return "[" + str + "]";
}
bool Token::equal(const Token &t) const {
	if(tag != t.tag) return false;
	if(lexeme.compare(t.lexeme) != 0) return false;
	return true;
}
std::vector<Token> getTokens(const std::string &expr) {
	std::vector<Token> t;
	auto i = expr.begin();
	while(i != expr.cend()) {
		while(*i == ' ' || *i == '\t')++i;
//std::cout<<"["<<peek<<"]";
		switch(*i) {
			case ';' :
				++i;
				t.push_back(Token(Tag::SCOL));
				break;
			case '%' :
				++i;
				t.push_back(Token(Tag::REM));
				break;
			case '#' :
				++i;
				t.push_back(Token(Tag::HASH));
				break;
			case '^' :
				++i;
				t.push_back(Token(Tag::POW));
				break;
			case '(' :
				++i;
				if(!t.empty() && t.back().tag == Tag::VAR)
					t.back().tag = Tag::FUN;
				t.push_back(Token(Tag::LPAR));
				break;
			case ')' :
				++i;
				t.push_back(Token(Tag::RPAR));
				break;
			case '{' :
				++i;
				t.push_back(Token(Tag::LBLK));
				break;
			case '}' :
				++i;
				t.push_back(Token(Tag::RBLK));
				break;
			case '/' :
				++i;
				t.push_back(Token(Tag::DIV));
				break;
			case '[' :
				++i;
				t.push_back(Token(Tag::LSQB));
				break;
			case ']' :
				++i;
				t.push_back(Token(Tag::RSQB));
				break;
			case '+' :
				++i;
				t.push_back(Token(Tag::PLUS));
				break;
			case '-' :
				++i;
				t.push_back(Token(Tag::MINUS));
				break;
			case '*' :
				++i;
				t.push_back(Token(Tag::MUL));
				break;
			case ',' :
				++i;
				t.push_back(Token(Tag::COMM));
				break;
			case '&' :
				if(*(i++) == '&') t.push_back(Token(Tag::LAND));
				else t.push_back(Token(Tag::BAND));
				break;
			case '|' :
				if(*(i++) == '|') t.push_back(Token(Tag::LOR));
				else t.push_back(Token(Tag::BOR));
				break;
			case '=' :
				if(*(i++) == '=') t.push_back(Token(Tag::EQ));
				else t.push_back(Token(Tag::ASSG));
				break;
			case '!' :
				if(*(i++) == '=') t.push_back(Token(Tag::NEQ));
				else t.push_back(Token(Tag::LNOT));
				break;
			case ':' :
				if(*(i++) == ':') t.push_back(Token(Tag::SRES));
				else t.push_back(Token(Tag::COL));
				break;
			case '<' :
				if(*(i++) == '=') t.push_back(Token(Tag::ELES));
				if(*i == '<') {
					//peek = ' ';
					t.push_back(Token(Tag::BLS));
				}
				else t.push_back(Token(Tag::LES));
				break;
			case '>' :
				if(*(i++) == '=') t.push_back(Token(Tag::EGRE));
				if(*i == '>') {
					//peek = ' ';
					t.push_back(Token(Tag::BRS));
				}
				else t.push_back(Token(Tag::GRE));
				break;
		}
		if(isdigit(*i)) {
			std::string b;
			do {
				b.push_back(*i);
				i++;
			}
			while(i != expr.cend() && isdigit(*i));
			if(*i != '.') {
				t.push_back(Token(b, Tag::REAL));
				continue;
			}
			b.push_back('.');
			i++;
			for(; i != expr.cend();) {
				if(!isdigit(*i))
					break;
				b.push_back(*i);
				i++;
			}
			t.push_back(Token(b, Tag::REAL));
		}
		else if(isalpha(*i)) {
			std::string b;
			do {
				b.push_back(*i);
				i++;
			}
			while(i != expr.cend() && (isalnum(*i) || *i == '_'));
			t.push_back(Token(b, Tag::VAR));
		}
	}
	return t;
}
