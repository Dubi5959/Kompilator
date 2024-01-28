%{

#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include "codeGenerator.hpp"
#include "symbolTable.hpp"

extern int yylex();
extern int yylineno;
extern FILE *yyin;

int yyerror(std::string str);

static std::string outputFile{};

%}

%code requires{
	#include "codeGenerator.hpp"
}

%union{
	std::string *pidentifier;
	long long num;
	bool boo;
	std::vector<std::string>* argsArgs;
	procedureHeader* procedureH;
	std::vector<instruction>* instructions;
	std::vector<std::pair<std::string*,bool>>* arguments;
}

%start program_all

%token PROCEDURE
%token PROGRAM
%token END
%token IS
%token IN
%token WHILE
%token DO
%token ENDWHILE
%token IF
%token THEN
%token ENDIF
%token READ
%token WRITE
%token ELSE
%token REPEAT
%token UNTIL
%token T
%token ASSIGN
%token EQUAL
%token NOTEQUAL
%token LESS
%token MORE
%token LESSEQUAL
%token MOREEQUAL
%token <pidentifier> pidentifier
%token <num> num
%token ERROR

%left ADD SUB
%left MULTI DIV MOD

%type <instructions> value
%type <instructions> identifier
%type <instructions> condition
%type <instructions> commands
%type <instructions> command
%type <instructions> expression
%type <instructions> proc_call
%type <instructions> procedures
%type <instructions> main
%type <procedureH> proc_head
%type <boo> declarations
%type <arguments> args_decl
%type <argsArgs> args

%%
program_all: procedures main {saveProgram(outputFile,$1,$2);}
	;

procedures: procedures PROCEDURE proc_head IS declarations IN commands END {$$ = procedures($1,$3,$7); setMain();}
	 | procedures PROCEDURE proc_head IS IN commands END {$$ = procedures($1,$3,$6); setMain();}
	 | {$$ = makeEmptyVec(); setMain();}
	 ;

main: PROGRAM IS declarations IN commands END  {$$ = $5;$$=addHalt($5);}
	 | PROGRAM IS IN commands END  {$$ = $4; $$=addHalt($4);}
	 ;
commands: commands command  {$$=merge($1,$2);}
	 | command {$$=$1;}
	 ;

command : identifier ASSIGN expression';'   			{$$ = assign($1,$3);}
	 | IF condition THEN commands ELSE commands ENDIF 	{$$ = ifElseFunction($2,$4,$6);}
	 | IF condition THEN commands ENDIF  			{$$ = ifFunction($2,$4);}
	 | WHILE condition DO commands ENDWHILE 		{$$ = whileDoFunction($2,$4);}
	 | REPEAT commands UNTIL condition';'			{$$ = repeatUntilFunction($4,$2);}
	 | proc_call';'             				{$$=$1;}
	 | READ identifier';'					{$$ = read($2); }
	| WRITE value';'					{$$ = write($2);}
	;

proc_head: pidentifier'('args_decl')' {$$ = proc_head_function($1,$3,yylineno);}
	;

proc_call: pidentifier'('args')' {$$ = proc_call_function($1,$3,yylineno);}
	;

declarations: declarations','pidentifier    		{varDeclaration($3,yylineno);}
	 | declarations','pidentifier'['num']'		{arrayDeclaration($3,$5,yylineno);}
	 | pidentifier					{varDeclaration($1,yylineno);}
	 | pidentifier'['num']'				{arrayDeclaration($1,$3,yylineno);}
	 ;


args_decl: args_decl','pidentifier 			{$$=args_decl_function($1,$3,false);}
	 | args_decl','T pidentifier			{$$=args_decl_function($1,$4,true);}
	 | pidentifier					{$$=args_decl_function($1,false);}
	 | T pidentifier				{$$=args_decl_function($2,true);}
	 ;

args: args','pidentifier				{$$=args_calling($1,$3);}
	 | pidentifier					{$$=args_calling($1);}
	 ;

expression: value					{$$=$1;}
	 | value ADD value				{$$=add($1,$3);}
	 | value SUB value				{$$=sub($1,$3);}
	 | value MULTI value			{$$=multi($1,$3);}
	 | value DIV value				{$$=division($1,$3);}
	 | value MOD value				{$$=mod($1,$3);}
	 ;

condition: value EQUAL value				{$$=isEqual($1,$3);}
	 | value NOTEQUAL value				{$$=isNotEqual($1,$3);}
	 | value MORE value				{$$=isMore($1,$3);}
	 | value LESS value				{$$=isLess($1,$3);}
	| value MOREEQUAL value				{$$=isMoreEqual($1,$3);}
	 | value LESSEQUAL value			{$$=isLessEqual($1,$3);}
	 ;

value: num				{$$=loadNumber($1);}
	 | identifier			{$1->push_back(instruction{std::string("LOAD d\n")});$$=$1;}
	 ;

identifier: pidentifier      {checkVar($1,false,yylineno); $$ = loadVar($1);}
	 | pidentifier'['num']'	{checkVar($1,true,yylineno); $$ = loadArray($1,$3);}
	 | pidentifier'['pidentifier']'{checkVar($1,true,yylineno); checkVar($3,false,yylineno); loadArray($1,$3);}
	 ;


%%

void error(std::string err, long long a){

}
	 
int main(int argv, char* argc[]){

	if( argv != 3 ) {
        cerr << "Error, podaj 3 argumenty: ./kompilator plik_wejsciowy plik_wyjsciowy" << endl;
        return 1;
    }
	outputFile = argc[2];
    yyin = fopen(argc[1], "r");
    if (yyin == NULL){
        cout << "Plik nie istnieje" << endl;
        return 1;
    }

	//cout << "tu działa\n";
	yyparse();


    cout << "Kompilacja zakonczona" << endl;

    return 0;
}

int yyerror(std::string err) {
    if (err == "syntax error") {
        err = "syntax error";
    }
    error(err, yylineno);
    return 1;
}

		