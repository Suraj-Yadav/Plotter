#ifndef TOKEN_H
#define TOKEN_H
#include <vector>
#include <string>
enum Tag {
	SOPER = 0,
	SRES, INCR, DECR, LPAR, RPAR, LBLK, RBLK, LSQB, RSQB, DOT, ARROW, PLUS, MINUS, LNOT, BNOT, ADDR, MUL, DIV, REM, BLS, BRS, LES, ELES, GRE, EGRE, EQ, NEQ, BAND, BXOR, BOR, LAND, LOR, QUES, COL, ASSG, COMM, HASH, SCOL, POW,
	EOPER,
	STRING, REAL, VAR, FUN, REST
};
struct Token {
	Tag tag; //!< Member variable "tag;"
	std::string lexeme; //!< Member variable "lexeme"
	explicit Token(Tag tage) {	tag = tage;}
	Token(std::string s, Tag tage);
	std::string toString();
	bool equal(const Token &t) const;
};
std::vector<Token> getTokens(const std::string &expr);
#endif // TOKEN_H
