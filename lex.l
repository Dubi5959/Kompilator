%option yylineno
%option nounput
%option noyywrap

%{
#include<iostream>
#include<string>
#include<vector>
#include"parser_tab.h"

%}

IDENTIFIER              [_a-z]
NUM                   	[0-9]
SIGN                   [\(\)\[\]\:\;,]


%%
"PROCEDURE"		{return PROCEDURE;}
"PROGRAM"                 { return PROGRAM; }
"IS"			{return IS;}
"IN"			{return IN;}
"END"                   { return END; }
"READ"                  { return READ; }
"WRITE"                 { return WRITE; }
"IF"                    { return IF; }
"THEN"                  { return THEN; }
"ELSE"                  { return ELSE; }
"ENDIF"                 { return ENDIF; }
"WHILE"                 { return WHILE; }
"DO"                    { return DO; }
"ENDWHILE"              { return ENDWHILE; }
"REPEAT"		{ return REPEAT; }
"UNTIL"			{ return UNTIL; }
"T "			{ return T;}
":="			{ return ASSIGN;} 
"+"			{ return ADD;}
"-"			{ return SUB;}
"*"			{ return MULTI;}
"/"			{ return DIV;}
"%"			{ return MOD;}
"="			{ return EQUAL;}
"!="			{ return NOTEQUAL;}
"<"			{ return  LESS;}
">"			{ return MORE;}
"<="			{ return LESSEQUAL;}
">="			{ return MOREEQUAL;}



{IDENTIFIER}+           { 
                            yylval.pidentifier = new std::string(yytext);
                            return pidentifier;
                        }
{NUM}+                { yylval.num = std::stoll(yytext); return num; }

{SIGN}                 { return yytext[0]; }

\#.*\n			{}

[ \t\r\n]+              {}
.                       { return ERROR; }

%%
