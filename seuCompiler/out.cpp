//%{ start
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

//%} end
//%! start
//%! end
//%% start
string yylex();
//%% end
//É¨Ãèº¯Êý
string LEX_scanner(char *str)
{
    char ch = ' ';
    while(ch != '\0')
    {
        //printf("%c %d\n", ch, LEX_STATE);
        switch(LEX_STATE) {
        case 0:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == ' '){
                LEX_STATE = 1;
            }
            else
            if(ch == '#'){
                LEX_STATE = 2;
            }
            else
            if(ch == '&'){
                LEX_STATE = 3;
            }
            else
            if(ch == '('){
                LEX_STATE = 4;
            }
            else
            if(ch == ')'){
                LEX_STATE = 5;
            }
            else
            if(ch == '*'){
                LEX_STATE = 6;
            }
            else
            if(ch == '+'){
                LEX_STATE = 7;
            }
            else
            if(ch == ','){
                LEX_STATE = 8;
            }
            else
            if(ch == '-'){
                LEX_STATE = 9;
            }
            else
            if(ch == '.'){
                LEX_STATE = 10;
            }
            else
            if(ch == '/'){
                LEX_STATE = 11;
            }
            else
            if(ch == ';'){
                LEX_STATE = 12;
            }
            else
            if(ch == '<'){
                LEX_STATE = 13;
            }
            else
            if(ch == '='){
                LEX_STATE = 14;
            }
            else
            if(ch == '>'){
                LEX_STATE = 15;
            }
            else
            if(ch == '['){
                LEX_STATE = 16;
            }
            else
            if(ch == '\"'){
                LEX_STATE = 17;
            }
            else
            if(ch == '\''){
                LEX_STATE = 18;
            }
            else
            if(ch == '\n'){
                LEX_STATE = 19;
            }
            else
            if(ch == '\t'){
                LEX_STATE = 20;
            }
            else
            if(ch == ']'){
                LEX_STATE = 21;
            }
            else
            if(ch == '^'){
                LEX_STATE = 22;
            }
            else
            if(ch == '_'){
                LEX_STATE = 23;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 24;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 25;
            }
            else
            if(ch == '{'){
                LEX_STATE = 26;
            }
            else
            if(ch == '|'){
                LEX_STATE = 27;
            }
            else
            if(ch == '}'){
                LEX_STATE = 28;
            }
            else
            if(ch == '~'){
                LEX_STATE = 29;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 1:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{}
//**************e
            }
            break;
        }
        case 2:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'i'){
                LEX_STATE = 30;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 3:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '&'){
                LEX_STATE = 31;
            }
            else
            if(ch == '='){
                LEX_STATE = 32;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 4:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 5:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 6:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 7:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '+'){
                LEX_STATE = 33;
            }
            else
            if(ch == '='){
                LEX_STATE = 34;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 24;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 8:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 9:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '-'){
                LEX_STATE = 35;
            }
            else
            if(ch == '='){
                LEX_STATE = 36;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 24;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 10:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 11:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '='){
                LEX_STATE = 37;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 12:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 13:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '<'){
                LEX_STATE = 38;
            }
            else
            if(ch == '='){
                LEX_STATE = 39;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 14:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '='){
                LEX_STATE = 40;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 15:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '='){
                LEX_STATE = 41;
            }
            else
            if(ch == '>'){
                LEX_STATE = 42;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 16:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 17:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '\"'){
                LEX_STATE = 43;
            }
            else
            if(isNoTow(ch)){
                LEX_STATE = 17;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 18:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '\''){
                LEX_STATE = 44;
            }
            else
            if(isNoOne(ch)){
                LEX_STATE = 18;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 19:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{}
//**************e
            }
            break;
        }
        case 20:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{}
//**************e
            }
            break;
        }
        case 21:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 22:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '='){
                LEX_STATE = 45;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 23:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '_'){
                LEX_STATE = 46;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 47;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 48;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{	int id = getKeyId(LEX_TEXT);	if(id != 0)	{	char* STR=LEX_TEXT;	char *orign=STR;    for (; *STR!='\0'; STR++)        *STR = toupper(*STR);		printf("<%s,->\n", orign);	storeresult("%s\n",orign);			}	else	{printf("<$ID,%s>\n", LEX_TEXT);	storeresult("IDENTIFIER\n",LEX_TEXT);			}}
//**************e
            }
            break;
        }
        case 24:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 49;
            }
            else
            if(ch == 'E'){
                LEX_STATE = 50;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 24;
            }
            else
            if(ch == 'e'){
                LEX_STATE = 51;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("NUMBER\n",LEX_TEXT);		printf("<$NUM,%s>\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 25:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '_'){
                LEX_STATE = 46;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 47;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 48;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{	int id = getKeyId(LEX_TEXT);	if(id != 0)	{	char* STR=LEX_TEXT;	char *orign=STR;    for (; *STR!='\0'; STR++)        *STR = toupper(*STR);		printf("<%s,->\n", orign);	storeresult("%s\n",orign);			}	else	{printf("<$ID,%s>\n", LEX_TEXT);	storeresult("IDENTIFIER\n",LEX_TEXT);			}}
//**************e
            }
            break;
        }
        case 26:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 27:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '='){
                LEX_STATE = 52;
            }
            else
            if(ch == '|'){
                LEX_STATE = 53;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 28:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 29:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '='){
                LEX_STATE = 54;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 30:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'n'){
                LEX_STATE = 55;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 31:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 32:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 33:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 34:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 35:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 36:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 37:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 38:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 39:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 40:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 41:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 42:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 43:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'STRING'\n",LEX_TEXT);		printf("<$STR,%s>\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 44:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'STRING'\n",LEX_TEXT);		printf("<$STR,%s>\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 45:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 46:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '_'){
                LEX_STATE = 46;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 47;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 48;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{	int id = getKeyId(LEX_TEXT);	if(id != 0)	{	char* STR=LEX_TEXT;	char *orign=STR;    for (; *STR!='\0'; STR++)        *STR = toupper(*STR);		printf("<%s,->\n", orign);	storeresult("%s\n",orign);			}	else	{printf("<$ID,%s>\n", LEX_TEXT);	storeresult("IDENTIFIER\n",LEX_TEXT);			}}
//**************e
            }
            break;
        }
        case 47:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '_'){
                LEX_STATE = 46;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 47;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 48;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{	int id = getKeyId(LEX_TEXT);	if(id != 0)	{	char* STR=LEX_TEXT;	char *orign=STR;    for (; *STR!='\0'; STR++)        *STR = toupper(*STR);		printf("<%s,->\n", orign);	storeresult("%s\n",orign);			}	else	{printf("<$ID,%s>\n", LEX_TEXT);	storeresult("IDENTIFIER\n",LEX_TEXT);			}}
//**************e
            }
            break;
        }
        case 48:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '_'){
                LEX_STATE = 46;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 47;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 48;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{	int id = getKeyId(LEX_TEXT);	if(id != 0)	{	char* STR=LEX_TEXT;	char *orign=STR;    for (; *STR!='\0'; STR++)        *STR = toupper(*STR);		printf("<%s,->\n", orign);	storeresult("%s\n",orign);			}	else	{printf("<$ID,%s>\n", LEX_TEXT);	storeresult("IDENTIFIER\n",LEX_TEXT);			}}
//**************e
            }
            break;
        }
        case 49:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(isDigit(ch)){
                LEX_STATE = 56;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 50:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(isDigit(ch)){
                LEX_STATE = 57;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 51:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(isDigit(ch)){
                LEX_STATE = 57;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 52:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 53:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 54:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("'%s'\n",LEX_TEXT);		printf("<%s,->\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 55:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'c'){
                LEX_STATE = 58;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 56:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'E'){
                LEX_STATE = 50;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 56;
            }
            else
            if(ch == 'e'){
                LEX_STATE = 51;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("NUMBER\n",LEX_TEXT);		printf("<$NUM,%s>\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 57:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(isDigit(ch)){
                LEX_STATE = 57;
            }
            else
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{storeresult("NUMBER\n",LEX_TEXT);		printf("<$NUM,%s>\n", LEX_TEXT);}
//**************e
            }
            break;
        }
        case 58:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'l'){
                LEX_STATE = 59;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 59:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'u'){
                LEX_STATE = 60;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 60:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'd'){
                LEX_STATE = 61;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 61:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'e'){
                LEX_STATE = 62;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 62:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == ' '){
                LEX_STATE = 63;
            }
            else
            if(ch == '"'){
                LEX_STATE = 64;
            }
            else
            if(ch == '<'){
                LEX_STATE = 65;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 63:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == ' '){
                LEX_STATE = 63;
            }
            else
            if(ch == '"'){
                LEX_STATE = 64;
            }
            else
            if(ch == '<'){
                LEX_STATE = 65;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 64:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '_'){
                LEX_STATE = 66;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 67;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 65:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '_'){
                LEX_STATE = 68;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 69;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 66:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 70;
            }
            else
            if(ch == '_'){
                LEX_STATE = 71;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 72;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 73;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 67:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 70;
            }
            else
            if(ch == '_'){
                LEX_STATE = 71;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 72;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 73;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 68:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 74;
            }
            else
            if(ch == '_'){
                LEX_STATE = 75;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 76;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 77;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 69:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 74;
            }
            else
            if(ch == '_'){
                LEX_STATE = 75;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 76;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 77;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 70:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'h'){
                LEX_STATE = 78;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 71:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 70;
            }
            else
            if(ch == '_'){
                LEX_STATE = 71;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 72;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 73;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 72:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 70;
            }
            else
            if(ch == '_'){
                LEX_STATE = 71;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 72;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 73;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 73:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 70;
            }
            else
            if(ch == '_'){
                LEX_STATE = 71;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 72;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 73;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 74:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == 'h'){
                LEX_STATE = 79;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 75:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 74;
            }
            else
            if(ch == '_'){
                LEX_STATE = 75;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 76;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 77;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 76:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 74;
            }
            else
            if(ch == '_'){
                LEX_STATE = 75;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 76;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 77;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 77:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '.'){
                LEX_STATE = 74;
            }
            else
            if(ch == '_'){
                LEX_STATE = 75;
            }
            else
            if(isDigit(ch)){
                LEX_STATE = 76;
            }
            else
            if(isLetter(ch)){
                LEX_STATE = 77;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 78:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '"'){
                LEX_STATE = 80;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 79:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            if(ch == '>'){
                LEX_STATE = 81;
            }
            else
            {
printf("Error in line %d\n", LEX_LINE);
exit(1);
            }
            break;
        }
        case 80:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{/*storeresult("%s\n",LEX_TEXT);printf("%s\n",LEX_TEXT);*/}
//**************e
            }
            break;
        }
        case 81:
        {
            ch = *str++;
            LEX_TEXT[LEX_TEXT_LEN++]=ch;
            {
LEX_TEXT[LEX_TEXT_LEN-1] = '\0';
LEX_TEXT_LEN=0;
LEX_STATE=0;
str--;
//**************s
{/*storeresult("%s\n",LEX_TEXT);printf("%s\n",LEX_TEXT);*/}
//**************e
            }
            break;
        }
        }
    }
    return "NULL";
}

string yylex()
{
   string thelex="NULL";
    FILE* infile;
    errno_t err = fopen_s(&infile, LEX_FILE_NAME, "r");
    while(NULL != fgets(LEX_BUFF, LEX_MAXSIZE_BUFF, infile))
    {
        ++LEX_LINE;
        thelex=LEX_scanner(LEX_BUFF);
    }
    fclose(file);
    return thelex;
}
