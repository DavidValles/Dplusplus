%code requires
{
    #include <string>
    using namespace std;
}

%{
#include <stdio.h>     
#include <stdlib.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <stack>

#include "util/variableTable.cpp"
#include "util/functionTable.cpp"
#include "util/constantTable.cpp"
#include "util/cube.cpp"
#include "util/quadruple.cpp"
#include "util/typeAdapter.cpp"
#include "util/operations.h"

using namespace std;

void yyerror (const char *s);
void insertConstantToTable(int &constantAddress);
void checkVariable();
void checkFunction();
bool checkConstant();
void checkOperator(int oper1, int oper2 = -1);
void returnProcess();

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

/*
    Utilities for functions and syntax checking
*/
FunctionTable functionTable;
string currentFunction;
int currentParameter;

/*
    Utilities for constants
*/
ConstantTable constantTable;

// Quadruples
Cube cube;
vector<Quadruple> quadruples;
TypeAdapter typeAdapter;
stack<int> typeStack;
stack<int> operandStack;
stack<int> operatorStack;
stack<int> jumpStack;
unordered_set<int> relationalOperators = { 
    Ops::GreaterThan,
    Ops::LessThan,
    Ops::NotEqualTo,
    Ops::EqualTo,
    Ops::LessThanOrEqualTo,
    Ops::GreaterThanOrEqualTo
};

int Integer = typeAdapter.getIntegerMin();
int Decimal = typeAdapter.getDecimalMin();
int Text = typeAdapter.getTextMin();
int Character = typeAdapter.getCharacterMin();
int Flag = typeAdapter.getFlagMin();
int Array = typeAdapter.getArrayMin();
int Matrix = typeAdapter.getMatrixMin();
int None = typeAdapter.getNoneMin();
int Avail = typeAdapter.getAvailMin();
int IntegerConstant = typeAdapter.getIntegerConstantMin();
int DecimalConstant = typeAdapter.getDecimalConstantMin();
int StringConstant = typeAdapter.getStringConstantMin();
int CharacterConstant = typeAdapter.getCharacterConstantMin();
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
start       :   {
                    Quadruple mainQ(Ops::Goto, -1, -1, -1);
                    jumpStack.push(quadruples.size());
                    quadruples.push_back(mainQ);
                }
                program
                {
                    cout<<"Main class compiled."<<endl;
                }
            | class
                { cout<<"User defined class compiled."<<endl; }
            ;

/*
    Basic program structure 
    A program can have includes, global variables and functions (0...*)
    It should always have a main method with a block
*/
program     : includes variable_section functions MAIN
                {
                    // Add goto jump for main quadruple
                    int mainGoTo = jumpStack.top();
                    jumpStack.pop();
                    quadruples[mainGoTo].result = quadruples.size();
                }
                block
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
                    typeAdapter.getNextAddress((*currentType));
                }
                variable_
            | ID 
                {
                    string id = *yylval.stringValue;
                    (*currTable).insertVariable(id, *currentType);
                    typeAdapter.getNextAddress((*currentType));
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
                        // Insert function to table
                        currentFunction = *yylval.stringValue;
                        functionTable.insertFunction(currentFunction, 
                                            typeAdapter.getType(*currentType),
                                            quadruples.size(), (*currentType)); 
                        typeAdapter.getNextAddress((*currentType));

                        // Switch scope to local table
                        currTable = &localTable;
                    }
                    '(' params ')' block
                    {
                        Function cFunction = 
                                functionTable.getFunction(currentFunction);

                        // Check last quadruple to determine if there was a
                        //      return value
                        if (cFunction.type != typeAdapter.getType(None) && 
                                quadruples[quadruples.size() - 1].oper != 
                                Ops::Endproc) {
                            cout<<"Error: function should have return value"<<
                                    endl;
                        }

                        // For debugging purposes
                        cout<<"Displaying function table"<<endl;
                        functionTable.displayTable();
	                    cout<<"Displaying local variable table"<<endl;
                        (*currTable).displayTable();

                        // Clear the local variable table and change to global
                        //      scope
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
params      : type 
                {
                    functionTable.addParameterToFunction(currentFunction, 
                                        typeAdapter.getType((*currentType))); 
                } 
                ID  
                {   
                    string id = *yylval.stringValue;
                    (*currTable).insertVariable(id, *currentType);
                    typeAdapter.getNextAddress((*currentType));
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
block       : '{' block_ return '}'
            ;

block_      : statement block_
            |
            ;

return      : RETURN expression
                {
                    if (currTable == &globalTable) {
                        cout<<"Error: cannot have return in main."<<endl;
                    }
                    returnProcess();
                }
                ';'
            |
            ;

/*
    Various statements available for blocks
*/
statement   : ID 
                {
                    checkVariable();
                    string id = *yylval.stringValue;
                    int address = (*currTable).getAddress(id);
                    int type = typeAdapter.getType(address);
                    operandStack.push(address);
                    typeStack.push(type);
                    operatorStack.push(Ops::Equal);
                }
                assignment 
                {
                    int oper = operatorStack.top();
                    if (oper == Ops::Equal) {
                        operatorStack.pop();
                        int assign = operandStack.top();
                        operandStack.pop();
                        int to = operandStack.top();
                        operandStack.pop();
                        int assignType = typeStack.top();
                        typeStack.pop();
                        int toType = typeStack.top();
                        typeStack.pop();

                        if (cube.cube[toType][assignType][Ops::Equal] == -1 ) {
                            cout<<"Error in assignment with type. Line "<<
                                    yylineno<<endl;
                        }
                        Quadruple quadruple(Ops::Equal, assign, -1, to);
                        quadruples.push_back(quadruple);
                    }
                }
                ';'
            | cycle
            | if 
            | print
            | read 
            | variables 
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
                {
                    insertConstantToTable(StringConstant);
                }
            | CCONSTANT 
                {
                    insertConstantToTable(CharacterConstant);
                }
            | functioncall 
            ;

/* 
    Function call structure. Can have 0 to n parameters.
*/

functioncall    : ID 
                    {
                        checkFunction();

                        // Set current values for parameter checking
                        currentFunction = *yylval.stringValue;
                        currentParameter = 0;
                        // TODO: El era deberia ser con un tag de la func
                        Quadruple eraQ(Ops::Era, functionTable.getFunction
                                (currentFunction).quadruple, -1, -1);
                        quadruples.push_back(eraQ);
                    }
                    '(' functioncall_ ')' 
                    {
                        Function cFunction =
                                functionTable.getFunction(currentFunction);

                        // Check if parameters in function call match 
                        //    the necessary in the declaration
                        if (functionTable.getParametersSize(currentFunction)
                                != currentParameter) {
                            cout<<"Incorrect number of parameters in function "
                                <<currentFunction<<" in line "<<yylineno
                                <<endl;
                        }

                        Quadruple goSubQ(Ops::GoSub, cFunction.quadruple,
                                -1, -1);
                        quadruples.push_back(goSubQ);

                        // Push the return value to operandStack
                        // Plus function address is moved to avail to deal with
                        //      recursive calls.
                        if (cFunction.type != typeAdapter.getType(None)) {
                            int tempReturn = Avail;
                            typeAdapter.getNextAddress(Avail);

                            Quadruple moveReturn(Ops::Equal,
                                    cFunction.returnAddress, -1, tempReturn);
                            quadruples.push_back(moveReturn);

                            operandStack.push(tempReturn);
                            typeStack.push(cFunction.type);
                        }
                    }
                ;

functioncall_   : expression 
                    {   
                        // Get type of the parameter from the expression
                        int address = operandStack.top(); 
                        operandStack.pop();
                        int type = typeStack.top();
                        typeStack.pop();

                        // Check if parameter matches type from function def
                        if (!functionTable.checkTypeOfParameter(
                                    currentFunction, type, currentParameter)) {
                            cout<<"Wrong parameter type in function "<<
                                currentFunction<<", parameter #"<<
                                currentParameter + 1<<" in line "<<yylineno<<endl;
                        }
                        Quadruple paramQ(Ops::Param, address, -1, -1);
                        quadruples.push_back(paramQ);
                        currentParameter++;
                    }
                    functioncall__
                |
                ;

functioncall__  : ',' functioncall_
                |
                ;

/*
    While and do while strucutre
*/
cycle       : DO
                {
                    int cont = quadruples.size();
                    jumpStack.push(cont);
                }
              block while
                {
                    int aux = typeStack.top();
                    if(aux != 4){
                        cout<<"Semantic Error";
                    }
                    else{
                        int gotoTrue = jumpStack.top();
                        jumpStack.pop();

                        int address = operandStack.top();
                        operandStack.pop();
                        typeStack.pop();
                        Quadruple quadruple(19, address, -1, gotoTrue);
                        quadruples.push_back(quadruple);
                    }
                }
            | while
                {
                    int cont = quadruples.size();
                    jumpStack.push(cont);

                    int aux = typeStack.top();
                    if(aux != 4){
                        cout<<"Semantic Error";
                    }
                    else{

                        int address = operandStack.top();
                        operandStack.pop();
                        typeStack.pop();
                        Quadruple quadruple(20, address, -1, -1);
                        quadruples.push_back(quadruple);

                        jumpStack.push(cont-1);
                    }
                }
             block
                {   
                    int returnTop = jumpStack.top();
                    jumpStack.pop();
                    Quadruple quadruple(18, -1, -1, returnTop);

                    int cont = quadruples.size();
                    int gotoFalse = jumpStack.top();
                    jumpStack.pop();
                    quadruples[gotoFalse].result = cont;
                }
            ;

while       : WHILE '(' expression ')'
            ;

/*
    If structure
*/
if          : IF if_ else
            ;

if_         : '(' expression ')' block else_if
            ;

else_if     : ELSEIF if_
            |
            ;

else        : ELSE block
            |
            ;

/*
    Print structure
*/
print       : PRINT '(' print_ ')' ';'
            ;

print_      : expression 
                {  
                    int address = operandStack.top();
                    operandStack.pop();
                    // TODO Check cube for print
                    Quadruple quadruple(Ops::Print, -1, -1, address);
                    quadruples.push_back(quadruple);
                } 
                print__
            | SCONSTANT print__
            ;

print__     : ',' print_
            |
            ;

/*
    Print structure
*/
read       : READ '(' read_ ')' ';'
            ;

read_      : ID 
                {  
                    checkVariable();
                    string id = *yylval.stringValue;
                    int address = (*currTable).getAddress(id);
                    Quadruple quadruple(Ops::Read, -1, -1, address);
                    quadruples.push_back(quadruple);
                } 
                read__
            ;

read__     : ',' read_
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
expression  : and
                {
                    checkOperator(Ops::Or);
                }
                expression_
            ;

expression_ : OR
                {
                    operatorStack.push(Ops::Or);
                }
                expression
            |
            ;

and         : numexp
                {
                    checkOperator(Ops::And);
                }
                and_
            ;

and_        : AND
                {
                    operatorStack.push(Ops::And);
                }
                and
            |
            ;

numexp      : NOT
                {
                    operatorStack.push(Ops::Not);
                    operandStack.push(-1);
                    typeStack.push(typeAdapter.getType(Flag));
                }
                exp numexp_ { checkOperator(Ops::Not); }
            | exp numexp_
            ;

numexp_     : '>' { operatorStack.push(Ops::GreaterThan); } 
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
            |
            ;

exp         : term 
                {
                    checkOperator(Ops::Sum, Ops::Minus);
                }
                exp_ 
            ;

exp_        : '+'
                {
                    operatorStack.push(Ops::Sum);
                }
                exp
            | '-'
                {
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
                    operatorStack.push(Ops::Multiplication);
                } 
                term
            | '/'
                {
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
                    
                    // Push operand and type to respective stacks
                    string id = *yylval.stringValue;
                    int address = (*currTable).getAddress(id);
                    int type = typeAdapter.getType(address);
                    typeStack.push(type);
                    operandStack.push(address);
                }
            | ICONSTANT 
                {
                    insertConstantToTable(IntegerConstant);
                }
            | DCONSTANT 
                {
                    insertConstantToTable(DecimalConstant);
                }
            ;

%%



/*
    Inserts a constant to the table of constants depending on its type
    It also pushes the constant and type to their stacks 
*/
void insertConstantToTable(int &constantAddress) {
    string id = *yylval.stringValue;
    if (!checkConstant()) {
        constantTable.insertConstant(id, constantAddress);
        typeAdapter.getNextAddress(constantAddress);
    }
    int address = constantTable.getAddress(id);
    int type = typeAdapter.getType(address);
    typeStack.push(type);
    operandStack.push(address);
}

/*
    Checks if the top operator of the stack matches the wanted operators 
        (the parameters).
    i.e. checkOperator(Ops::Multiplication, Ops::Division)

    Also can be called without parameters to check if the operator belongs to
        the relationalOperators like <, >
*/
void checkOperator(int oper1, int oper2) {
    if (!operatorStack.empty()) {
        int oper = operatorStack.top();
        if (oper == oper1 || oper == oper2 || 
                relationalOperators.find(oper) != relationalOperators.end()) {
            operatorStack.pop();
            int type2 = typeStack.top();
            typeStack.pop();
            int type1 = typeStack.top();
            typeStack.pop();
            int resultType = cube.cube[type1][type2][oper];

            // Check if result is valid
            if (resultType != -1) {
                int operand2 = operandStack.top();
                operandStack.pop();
                int operand1 = operandStack.top();
                operandStack.pop();
                Quadruple quadruple(oper, operand1, operand2, 
                                        Avail);
                quadruples.push_back(quadruple);
                operandStack.push(Avail);    
                typeAdapter.getNextAddress(Avail);
                typeStack.push(resultType);
            }
            else {
                cout<<"Type mismatch in line: "<<yylineno<<endl;
            }
        }
    }
}

/*
    Checks if the variable id in the current yylval has been declared
*/
void checkVariable() {
    string id = *yylval.stringValue;
    if (!(*currTable).findVariable(id)) {
        cout<<"Error! Line: "<<yylineno<<". Variable not defined: "<<id<<
            "."<<endl;
    }
}

/*
    Checks if the function id in the current yylval has been declared
*/
void checkFunction() {
    string id = *yylval.stringValue;
    if(!functionTable.findFunction(id)) {
        cout<<"Error! Line: "<<yylineno<<". Function not defined: "<<
            id<<"."<<endl;
    }
}

/*
    Checks if the constant id in the current yylval has been added to the
        constant table
*/
bool checkConstant() {
    string id = *yylval.stringValue;
    bool check = constantTable.findConstant(id);
    return check;
}

void returnProcess() {
    Function function =
            functionTable.getFunction(currentFunction);

    if (function.type == typeAdapter.getType(None)) {
        cout<<"None functions cannot have a return statement"
                <<endl;
    }

    int returnAddress = operandStack.top();
    operandStack.pop();
    int returnType = typeStack.top();
    typeStack.pop();

    if (function.type != returnType) {
        cout<<"Error in line "<<yylineno<<
                ": Incorrect return type."<<endl;
    }

    Quadruple assignReturn(Ops::Equal, returnAddress, -1,
            function.returnAddress);
    quadruples.push_back(assignReturn);

    operandStack.push(function.returnAddress);
    typeStack.push(function.type);

    //Generate return quadruple
    Quadruple returnQ(Ops::Endproc, -1, -1, -1);
    quadruples.push_back(returnQ);
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

	cout<<"Displaying quadruples"<<endl;
    for (int i=0; i<quadruples.size(); i++) {
        quadruples[i].display();
    }
	
    return 0;
}

void yyerror (const char *s) {
    fprintf (stderr, "%s at line %d.\n", s, yylineno);
}
