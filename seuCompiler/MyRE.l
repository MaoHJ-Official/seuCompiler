digit 	[0-9]
letter	[a-zA-Z]
id	{letter}({letter}|{digit})*;
number	{digit}+(.{digit}+)?(E[+-]?{digit}+)?;

%{
#include <string>
#include <iostream>
int line = 0;
int numCount = 0;
int idCount = 0;
%}

%%

"/*" { comment(); }
"//"[^\n]* { /* //-comment */ }
int {return "int";}
float {return "float";}
double {return "double";}
void {return "void";}
if {return "if";}
else {return "else";}
while {return "while";}
return {return "return";}
static {return "static";}
{id} {++idCount; return "id";}
{number} {++numCount; return "number";}
"+" {return "+";}
"-" {return "-";}
"*" {return "*";}
"/" {return "/";}
"%" {return "%";}
"=" {return "=";}
"+=" {return "+=";}
"-=" {return "-=";}
"*=" {return "*=";}
"/=" {return "/=";}
"%=" {return "%=";}
"++" {return "++";}
"--" {return "--";}
"&&" {return "&&";}
"||" {return "||";}
"!" {return "!";}
"<=" {return "<=";}
">=" {return ">=";}
"!=" {return "!=";}
"==" {return "==";}
"<" {return "<";}
">" {return ">";}
"(" {return "(";}
")" {return ")";}
"{" {return "{";}
"}" {return "}";}
"[" {return "[";}
"]" {return "]";}
";" {++line; return ";";}
"," {return ",";}

%%

void comment(void)
{
	char c, prev = 0;
  
	while ((c = input()) != 0)
	{
		if (c == '/' && prev == '*')
			return;
		prev = c;
	}
	std::cerr << "unterminated comment";
}

void main() {
	std::cout << "number of ";" : " << line << std::endl;
	std::cout << "count of num : " << numCount << std::endl;
	std::cout << "count of id : " << idCount << std::endl;
}