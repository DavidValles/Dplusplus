%code requires
{
    #include <string>
    using namespace std;
}

%{
#include <stdio.h>     
#include <stdlib.h>

#include <unordered_map>
#include <vector>
#include <iostream>

using namespace std;

void yyerror (const char *s);

extern "C"
{
    int yylex();
}

extern int yylineno;
unordered_map<string, int> varTable;
vector<string> variables;

enum Type {
    Int = 1,
    Float = 2
};

%}

%start start
%token INCLUDE
%token MAIN
%token RETURN
%token NONE 
                              
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
%token CCONSTANT 

%union {
  string* stringValue;
  float floatValue;
  int intValue;
} 

%%

/*
    A file in D++ can either be a program file with a main function or a 
    class file with a class definition.
*/
start       : program
            { cout<<"Main class: correct."<<endl; }
            | class
            { cout<<"User defined class: correct."<<endl; }
            ;

/*
    Basic program structure 
    A program can have includes, global variables and functions (0...*)
    It should always have a main method with a block
*/
program     : includes variable_section functions MAIN block 
            ;

/*
    Include section for global scope
    E.g. include "myLibrary.h"
*/
includes    : INCLUDE SCONSTANT includes 
            |
            ;

/*
    Variable section for global scope
    E.g. var integer id1;
*/
variable_section    : variables variable_section
                    |
                    ;

/*
    Declaring one or more variables in one line
    E.g. var integer id1, id2 = 3, id3 = 2 + 23 - id1;
*/
variables   : VAR type variable ';' 
            ;

variable    : ID variable_
            | assignment variable_
            ;

variable_   : ',' variable
            |
            ;

/*
    Function section for global scope
    E.g.
*/
functions   : singlefunction functions
            |
            ;

singlefunction  : FUNC singlefunction_ ID '(' params ')' block 
                ;

singlefunction_ : type
                | NONE
                ;

/*
    Declaring zero to n paramaters
*/
params      : type ID params_ 
            |
            ;

params_     : ',' params
            |
            ;

/*
    Typical block of code with statements   
*/
block       : '{' block_ '}'
            ;

block_      : statement block_
            |
            ;

/*
    Various statements available for blocks
*/
statement   : assignment ';'
            | cycle
            | if 
            | print
            | READ ID ';'
            | variables 
            | RETURN ID ';'
            ;

/*
    Assignments can have expressions, a string " ", 
        and a character ' '
*/
assignment  : ID '=' assignment_
            ;

assignment_ : expression
            | SCONSTANT 
            | CCONSTANT 
            ;

/*
    While and do while strucutre
*/
cycle       : DO block while
            | while block
            ;

while       : WHILE '(' condition ')'
            ;

/*
    If structure
*/
if          : IF if_ else
            ;

if_         : '(' condition ')' block else_if
            ;

else_if     : ELSEIF if_
            |
            ;

else        : ELSE block
            |
            ;

/*
    Conditional structure
*/
condition   : not expression condition_
            ;

condition_  : AND condition
            | OR condition
            |
            ;

not         : NOT
            |
            ;

/*
    Print structure
*/
print       : PRINT '(' print_ ')' ';'
            ;

print_      : expression print__
            | SCONSTANT print__
            ;

print__     : ',' print_
            |
            ;

/*
    Class structure
*/

class       : CLASS ID '{' classblock '}' 
            ;

classblock  : classblock_ classblock
            |
            ;

classblock_ : PRIVATE ':'
            | PUBLIC ':'
            | variables
            | singlefunction
            ;

/*
    Various accepted types 
*/
type        : INT 
            | DECIMAL 
            | TEXT 
            | CHARACTER  
            | FLAG
            ;


/*
    Basic expression structure 
*/
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

exp         : term exp_ 
            ;

exp_        : '+' exp
            | '-' exp
            | 
            ;

term        : factor term_ 
            ;

term_       : '*' term
            | '/' term
            | 
            ;

factor      : '(' expression ')' 
            | constvar 
            | '+' constvar
            | '-' constvar
            ;

constvar    : ID 
            | ICONSTANT 
            | DCONSTANT 
            ;

%%

/* 
    For variable table (currently in dev)
*/
void checkVariable(string id) {
    if (varTable.find(id) == varTable.end()) {
        cout<<"Undefined variable: "<<id<<endl;
    }
}

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
