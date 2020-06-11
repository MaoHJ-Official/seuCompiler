%{
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<ctype.h>
using namespace std;
#define LEX_MAXSIZE_TEXT 120
#define LEX_MAXSIZE_BUFF 1024

char LEX_FILE_NAME[100] = "test.c";
char LEX_OUT_FILE_NAME[100] = "result.txt";
int LEX_LINE = 0;
int LEX_STATE = 0;
int LEX_TEXT_LEN = 0;
char LEX_TEXT[LEX_MAXSIZE_TEXT];
char LEX_BUFF[LEX_MAXSIZE_BUFF];
FILE* file;
errno_t err = fopen_s(&file, LEX_OUT_FILE_NAME, "w");
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
void storeresult(const char* a,char* b)
{
   
    if (file == NULL)
    {
        printf("Can't open the file!\n");
    }
    fprintf(file, a, b);


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
char *strupr(char *str)
{
    char *orign=str;
    for (; *str!='\0'; str++)
        *str = toupper(*str);
    return orign;
}
int isNoTow(char ch)
{
	if(ch != '\"' && ch != '\0')
		return 1;
	return 0;
}
void lextotxt()
{

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
	{
	char* STR=LEX_TEXT;
	char *orign=STR;
    for (; *STR!='\0'; STR++)
        *STR = toupper(*STR);
	
	printf("<%s,->\n", orign);
	storeresult("%s\n",orign);
	
		}
	else
	{printf("<$ID,%s>\n", LEX_TEXT);
	storeresult("IDENTIFIER\n",LEX_TEXT);
		
	}
}
%$
(+|-|%$){digit}{digit}*(.{digit}{digit}*|%$)((E|e){digit}{digit}*|%$) {
storeresult("NUMBER\n",LEX_TEXT);
	
	printf("<$NUM,%s>\n", LEX_TEXT);
}
%$
%(|%)|%{|%}|[|]|;|,|. {
storeresult("'%s'\n",LEX_TEXT);
	
	printf("<%s,->\n", LEX_TEXT);
}
%$
% |{\t}|{\n} {

}
%$
(%*(%=|%$))|(/(=|%$))|(+(+|=|%$))|(-(-|=|%$))|(<(<|=|%$))|(>(>|=|%$))|(=(=|%$))|(&(&|=|%$))|(%|(%||=|%$))|(^(=|%$))|(~(=|%$)) {
storeresult("'%s'\n",LEX_TEXT);
	
	printf("<%s,->\n", LEX_TEXT);
}
%$
({\'}{noone}*{\'})|({\"}{notow}*{\"}) {
storeresult("'STRING'\n",LEX_TEXT);
	
	printf("<$STR,%s>\n", LEX_TEXT);
}
%$
#include(% )*((<({letter}|_)({letter}|_|{digit})*.h>)|("({letter}|_)({letter}|_|{digit})*.h")) {
/*storeresult("%s\n",LEX_TEXT);
printf("%s\n",LEX_TEXT);*/
}
%%