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
#include <stack>
#include "util/variableTable.cpp"
#include "util/functionTable.cpp"
#include "util/cube.cpp"
#include "util/typeAdapter.cpp"

using namespace std;

void yyerror (const char *s);
void checkVariable();
void checkFunction();

extern "C"
{
    int yylex();
}

extern int yylineno;

/*
    Utilities to find identifiers
    currTable indicates the curren table in scope
*/
VariableTable globalTable;
VariableTable localTable(&globalTable);
VariableTable* currTable = &globalTable;
int* currentType;

FunctionTable fglobalTable;
FunctionTable* fcurrTable = &fglobalTable;

enum Ops {
    Sum = 0;
    Minus = 1;
    Division = 2;
    Multiplication = 3;
    Modulo = 4;
    GreaterThan = 5;
    LessThan = 6;
    Equal = 7;
    And = 8;
    Or = 9;
    Not = 10;
    NotEqualto = 11;
    EqualTo = 12;
    LessThanOrEqualTo = 13;
    GreaterThanOrEqualTo = 14;
    Floor = 15;
}

// Quadruples
Cube cube;
stack<int> typeStack;
stack<int> operandStack;
stack<int> operatorStack;
vector<int> relationalOperators = { 
    Ops::GreaterThan,
    Ops::LessThan,
    Ops::Equal,
    Ops::NotEqualto,
    Ops::EqualTo,
    Ops::LessThanOrEqualTo,
    Ops::GreaterThanOrEqualTo,
}

int Integer = typeAdapter.getIntegerMin();
int Decimal = typeAdapter.getDecimalMin();
int Text = typeAdapter.getTextMin();
int Character = typeAdapter.getCharacterMin();
int Flag = typeAdapter.getFlagMin();
int Array = typeAdapter.getArrayMin();
int Matrix = typeAdapter.getMatrixMin();
int None = typeAdapter.getNoneMin();
int Avail = typeAdapter.getAvailMin();
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
                { cout<<"Main class compiled."<<endl; }
            | class
                { cout<<"User defined class compiled."<<endl; }
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
    E.g. include "myClass.h"
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

variable    : ID 
                {
                    string id = *yylval.stringValue;
                    (*currTable).insertVariable(id, *currentType);
                    (*currentType)++;
                }
                variable_
            | ID 
                {
                    string id = *yylval.stringValue;
                    (*currTable).insertVariable(id, *currentType);
                    (*currentType)++;
                }
                assignment variable_
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

singlefunction  : FUNC singlefunction_ ID 
                    {   
                        string id = *yylval.stringValue;
                        (*fcurrTable).insertFunction(id, *currentType); 
                        (*currentType)++;
                        currTable = &localTable;
                    }
                    '(' params ')' block 
                    {
	                    cout<<"Displaying local variable table"<<endl;
                        (*currTable).displayTable();
                        (*currTable).clearVarTable();
                        currTable = &globalTable;
                    }
                ;

singlefunction_ : type
                | NONE { currentType = &None; }
                ;

/*
    Declaring zero to n paramaters
*/
params      : type ID  
                {   
                    string id = *yylval.stringValue;
                    (*currTable).insertVariable(id, *currentType);
                    (*currentType)++;
                }
                params_ 
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
statement   : ID 
                {
                    checkVariable();
                }
                assignment ';'
            | cycle
            | if 
            | print
            | READ ID 
                {
                    checkVariable();
                }
                ';'
            | variables 
            | RETURN ID 
                {
                    checkVariable();
                }
                ';'
            | functioncall ';'
            ;

/*
    Assignments can have expressions, a string " ", 
        and a character ' '
*/
assignment  : '=' assignment_
            ;

assignment_ : expression
            | SCONSTANT 
            | CCONSTANT 
            | functioncall
            ;

/* 
    Function call structure. Can have 0 to n parameters.
*/

functioncall    : ID 
                    {
                        checkFunction();
                    }
                    '(' functioncall_ ')'
                ;

functioncall_   : ID 
                    {   
                        checkVariable();
                    }
                    functioncall__
                |
                ;

functioncall__    : ',' functioncall_
                |
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
type        : INT { currentType = &Integer; }
            | DECIMAL { currentType = &Decimal; }
            | TEXT { currentType = &Text; }
            | CHARACTER  { currentType = &Character; }
            | FLAG { currentType = &Flag; }
            | ARRAY '(' ICONSTANT ')' { currentType = &Array; }
            | MATRIX '(' ICONSTANT ',' ICONSTANT ')' 
                { currentType = &Matrix; }
            ;


/*
    Basic expression structure 
*/
expression  : exp expression_
            ;

expression_ : 
            | '>' { operatorStack.push(Ops::GreaterThan); } 
                exp
                { checkOperator(Ops::GreaterThan); }
            | '<' { operatorStack.push(Ops::LessThan); } 
                exp
                { checkOperator(Ops::LessThan); }
            | LESSTHANOREQUALTO 
                { operatorStack.push(Ops::LessThanOrEqualTo); } 
                exp
                { checkOperator(Ops::LessThanOrEqualTo); }
            | GREATERTHANOREQUALTO 
                { operatorStack.push(Ops::GreaterThanOrEqualTo); } 
                exp
                { checkOperator(Ops::GreaterThanOrEqualTo); }
            | EQUALTO { operatorStack.push(Ops::EqualTo); } 
                exp
                { checkOperator(Ops::EqualTo); }
            | NOTEQUALTO { operatorStack.push(Ops::NotEqualTo); } 
                exp
                { checkOperator(Ops::NotEqualTo); }
            ;

exp         : term 
                {
                    checkOperator(Ops::Sum, Ops::Minus);
                }
                exp_ 
            ;

exp_        : '+'
                {
                    // Push operator 
                    operatorStack.push(Ops::Sum);
                }
                exp
            | '-'
                {
                    // Push operator 
                    operatorStack.push(Ops::Minus);
                } 
                exp
            | 
            ;

term        : factor 
                {
                    checkOperator(Ops::Multiplication, Ops::Division);
                }
                term_ 
            ;

term_       : '*'
                {
                    // Push operator 
                    operatorStack.push(Ops::Multiplication);
                } 
                term
            | '/'
                {
                    // Push operator 
                    operatorStack.push(Ops::Division);
                }
                term
            | 
            ;

factor      :   {
                    operatorStack.push(Ops::Floor);
                }
                '(' expression ')' 
                {
                    operatorStack.pop();
                }
            | constvar 
            | '+' constvar
            | '-' constvar
            ;

constvar    : ID 
                {
                    checkVariable();
                    // Push operand and type
                    int address = (*currTable).getAddress(id);
                    int type = typeAdapter.getType(address);
                    typeStack.push(type);
                    operandStack.push(address);
                }
            | ICONSTANT 
            | DCONSTANT 
            ;

%%

void checkOperator(char c1, char c2 = -1) {
    char oper = operatorStack.top();
    operatorStack.pop();
    if (oper == c1 || oper == c2 || 
            relationalOperators.find(oper) != realtionalOperators.end()) {
        int type2 = typeStack.top();
        typeStack.pop();
        int type1 = typeStack.top();
        typeStack.pop();
        int resultType = cube.cube[type1][type2][oper];

        if (resultType != -1) {
            int operand2 = operandStack.top();
            operandStack.pop();
            int operand1 = operandStack.top();
            operandStack.pop();
            Quadruple quadruple(oper, operand1, operand2, 
                                    (*Avail));
            quadruple.display();
            operandStack.push_back(*Avail);    
            (*Avail)++;
            typeStack.push_back(resultType);
        }
        else {
            cout<<"Type mismatch in +"<<endl;
        }
    }
}

void checkVariable() {
    string id = *yylval.stringValue;
    if (!(*currTable).findVariable(id)) {
        cout<<"Error! Line: "<<yylineno<<". Variable not defined: "<<id<<
            "."<<endl;
    }
}

void checkFunction() {
    string id = *yylval.stringValue;
    if(!(*fcurrTable).findFunction(id)) {
        cout<<"Error! Line: "<<yylineno<<". Function not defined: "<<id<<"."<<endl;
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
    
	cout<<"Displaying global variable table"<<endl;
    (*currTable).displayTable();

    
    cout<<"Displaying global function table"<<endl;
    (*fcurrTable).displayTable();
    
	
    return 0;
}

void yyerror (const char *s) {
    fprintf (stderr, "%s at line %d.\n", s, yylineno);
}
