#include"Lex.h"
#include<map>
#include<vector>
#include<iostream>

using namespace std;
int main()
{
	Lex lex("RE.l", "out.c");
	lex.work();
	return 0;
}