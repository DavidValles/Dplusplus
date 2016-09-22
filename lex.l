%{
#include "y.tab.h"
#include <string>
#include <iostream>

extern "C" int yylex();
extern void yyerror (const char *s);
%}

%%

"include"					return INCLUDE;
"main"						return MAIN;
"return"					return RETURN;
"none"					 	return NONE;

"var"                       return VAR;
"func"						return FUNC;
"integer"                   return INT;
"decimal"                   return DECIMAL;
"text"						return TEXT;
"character"					return CHARACTER;
"flag"					 	return FLAG;
"array"					 	return ARRAY;
"matrix"					return MATRIX;

"if"                        return IF; 
"else"                      return ELSE; 
"else if"					return ELSEIF;

"while"						return WHILE;
"do"						return DO;

"class"						return CLASS;
"private"					return PRIVATE;
"public"					return PUBLIC;

"print"                     return PRINT; 
"read"                      return READ; 

"!="                        return NOTEQUALTO; 
"=="                        return EQUALTO; 
"<="                        return LESSTHANOREQUALTO; 
">="                        return GREATERTHANOREQUALTO; 
"and"                       return AND; 
"or"                        return OR; 
"not"                       return NOT; 

[a-zA-Z][_a-zA-Z0-9]*       {
    						yylval.stringValue = new std::string(yytext, yyleng); 
							return ID; 
			    			}

0|[1-9][0-9]*               return ICONSTANT; 
(0|[1-9][0-9]*)\.[0-9]*     return DCONSTANT; 
\"(\\.|[^"])*\"             return SCONSTANT; 
\'.\' 			            return CCONSTANT; 

[\{\}\=\:\;\,\.\+\-\*\/\<\>\(\)]  return yytext[0];

[\n]                        {yylineno++;};
[ \t]                       ; 
.                           { cout<<yytext<<endl; yyerror ("Unexpected character"); }

%%

int yywrap(void)
{
    return 1;
}
