%{
/* 
 * By Xue Yinsong,Student No.5120309415
 * this file is the lex source file , where all the tokens are being parsed here.
 * Mainly the idea was simple, we follow the instructions and get the tokens in the tutorial.
 * But in order to eliminate bugs, I have done a few changes:
 * 1.the token BINARYOP is splited to several tokens like add/product.....
 * 2.the char "-" is singled out as token SUB, because it has 2 roles(a binaryop and a unaryop)
 * 3.the token ASSIGN which was not declared yet used in the tutorial is added.
 * 4.the white characters are ignored.
 * 5.add a line count(curr_line), whenever a new line ends, it will increase by 1.
 */ 
#include <stdio.h>
	void yyerror(char *);
	int curr_line = 1;
	char buf[100];
	char tbuf[100];
extern YYSTYPE yylval;
%}


%%
(0[xX]?)?[0-9]+ {
	if (yytext[0] == '0'){
		if (yytext[1] == 'x' || yytext[1] == 'X')
			sscanf(yytext,"%x",&yylval.Intval);
		else
			sscanf(yytext,"%o",&yylval.Intval);
			
	}else
		sscanf(yytext,"%d",&yylval.Intval);
	return INT;
}
"int" {yylval.Type = new_int_type();return INTTYPE;}
"struct" {return STRUCT;}
"return" {return RETURN;;}
"if" return IF;
"else" return ELSE;
"break" return BREAK;
"continue" return CONT;
"for" return FOR;
[_a-zA-Z]([_a-zA-Z0-9])* {
	yylval.Strval = malloc(sizeof(char)*100);
	strcpy(yylval.Strval,yytext);
	return ID;
}
";" return SEMI;
"," return COMMA;
"(" return LP;
")" return RP;
"[" return LB;
"]" return RB;
"{" return LC;
"}" return RC;
"." return DOT;
"*"|"/"|"%" {
	yylval.Strval = malloc(sizeof(char)*100);
	strcpy(yylval.Strval,yytext);
	return PRODUCT;
}
"+" return ADD;
"-" return SUB;
"<<"|">>" {
	yylval.Strval = malloc(sizeof(char)*100);
	strcpy(yylval.Strval,yytext);
	return SHIFT;
}
">"|">="|"<"|"<="|"=="|"!=" {
	yylval.Strval = malloc(sizeof(char)*100);
	strcpy(yylval.Strval,yytext);
	return RELATION;
}
"&" return AND;
"^" return XOR;
"|" return OR;
"&&" return LAND;
"||" return LOR;
"="|"+="|"-="|"*="|"/="|"&="|"^="|"|="|"<<="|">>=" {
	yylval.Strval = malloc(sizeof(char)*100);
	strcpy(yylval.Strval,yytext);
	return ASSIGN;
}
"!"|"++"|"--"|"~" {
	yylval.Strval = malloc(sizeof(char)*100);
	strcpy(yylval.Strval,yytext);
	return UNARYOP;
}

[" "\t] ; 
[\n] curr_line++;

. {
	strcpy(buf,"syntax error:");
	strcat(buf,yytext);
	yyerror(buf);
}
%%
int yywrap(void) {
	return 1;
}
