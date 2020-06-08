%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int KEY_NUM = 32;
const char* KEY_SET[] = {
    "auto", "break", "case", "char", "const", "continue",
	"default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if", "int", "long", "register",
	"return", "short", "signed", "sizeof", "static", "struct",
	"switch", "typedef", "union", "unsigned", "void", "volatile",
	"while"
};

int isDigit(char ch)
{
	if(ch <= '9' && ch >= '0')
		return 1;
	return 0;
}

int isLetter(char ch)
{
	if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return 1;
	return 0;
}

int getKeyId(char *str)
{
	for(int i=0; i<KEY_NUM; i++)
	{
		if(strcmp(KEY_SET[i], str) == 0)
			return i+1;
	}
	return 0;
}

int isNoOne(char ch)
{
	if(ch != '\'' && ch != '\0')
		return 1;
	return 0;
}

int isNoTow(char ch)
{
	if(ch != '\"' && ch != '\0')
		return 1;
	return 0;
}

%}

%!
noone=isNoOne
notow=isNoTow
letter=isLetter
digit=isDigit
%!


%%
({letter}|_)({letter}|_|{digit})* {
	int id = getKeyId(LEX_TEXT);
	if(id != 0)
		printf("<%s,->\n", LEX_TEXT);
	else
	{
		printf("<$ID,%s>\n", LEX_TEXT);
	}
}
%$
(+|-|%$){digit}{digit}*(.{digit}{digit}*|%$)((E|e){digit}{digit}*|%$) {
	printf("<$NUM,%s>\n", LEX_TEXT);
}
%$
%(|%)|%{|%}|[|]|;|,|. {
	printf("<%s,->\n", LEX_TEXT);
}
%$
% |{\t}|{\n} {

}
%$
(%*(%=|%$))|(/(=|%$))|(+(+|=|%$))|(-(-|=|%$))|(<(<|=|%$))|(>(>|=|%$))|(=(=|%$))|(&(&|=|%$))|(%|(%||=|%$))|(^(=|%$))|(~(=|%$)) {
	printf("<%s,->\n", LEX_TEXT);
}
%$
({\'}{noone}*{\'})|({\"}{notow}*{\"}) {
	printf("<$STR,%s>\n", LEX_TEXT);
}
%$
#include(% )*((<({letter}|_)({letter}|_|{digit})*.h>)|("({letter}|_)({letter}|_|{digit})*.h")) {
	printf( LEX_TEXT);
}
%%