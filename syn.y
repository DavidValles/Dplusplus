%code requires
{
    #include <string>
    using namespace std;
}

%{
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>

#include <unordered_map>
#include <iostream>

using namespace std;

void yyerror (const char *s);

extern "C"
{
    int yylex();
}

extern int yylineno;
unordered_map<string, int> varTable;
string variables [20];
int cVar = 0;

enum Type {
    Int = 1,
    Float = 2
};

%}

%start start
%token INCLUDE
%token MAIN
                              
%token VAR
%token FUNC
%token INT
%token DECIMAL
%token TEXT
%token CHARACTER
%token FLAG
%token ARRAY
%token MATRIX
                              
%token IF 
%token ELSE 
%token ELSEIF
                              
%token WHILE
%token DO
                              
%token CLASS
%token PRIVATE
%token PUBLIC
                              
%token PRINT 
%token READ 
                              
%token NOTEQUALTO 
%token EQUALTO 
%token LESSTHANOREQUALTO 
%token GREATERTHANOREQUALTO 
%token AND 
%token OR 
%token NOT 

%token ID 
%token ICONSTANT 
%token DCONSTANT 
%token SCONSTANT 

%union {
  string* stringValue;
  float floatValue;
  int intValue;
} 

%%

start       : includes variables { cout<<"Apropiado."<<endl; }
            ;

includes	: INCLUDE SCONSTANT includes 
		 	|
			;

variables 	: VAR type variable ';' variables
		    |
			;

variable	: ID variable_
		 	| assignment variable_
			;

variable_ 	: ',' variable
		   	|
			;

type        : INT {
			for (int i=0; i<cVar; i++) {
			    varTable[variables[i]] = Int;
			}
			cVar = 0;
		    }
            | DECIMAL {
			for (int i=0; i<cVar; i++) {
			    varTable[variables[i]] = Float;
			}
			cVar = 0;
		    }
            ;

assignment 	: ID '=' expression
			| ID '=' SCONSTANT

expression  : exp expression_
            ;

expression_ : 
            | '>' exp
            | '<' exp
            | LESSTHANOREQUALTO exp
            | GREATERTHANOREQUALTO exp
            | EQUALTO exp
            | NOTEQUALTO exp
            ;

exp         : termino exp_ 
            ;

exp_        : '+' exp
            | '-' exp
            | 
            ;

termino     : factor termino_ 
            ;

termino_    : '*' termino
            | '/' termino
            | 
            ;

factor      : '(' expression ')' 
            | constvar 
            | '+' constvar
            | '-' constvar
            ;

constvar    : ID { 
				string id = *yylval.stringValue;
				if (varTable.find(id) == varTable.end()) {
					cout<<"Undefined variable: "<<id<<endl;
				}
			}
            | ICONSTANT 
            | DCONSTANT 
            ;

%%

int main(int argc, char **argv)
{
	if ((argc > 1) && (freopen(argv[1], "r", stdin) == NULL))
	{
		printf("Could not open file.\n");
		exit( 1 );
	}
	
	yyparse();
	
	return 0;
}

void yyerror (const char *s) {
    fprintf (stderr, "%s at line %d.\n", s, yylineno);
}
