#include"Lex.h"
#include<map>
#include<vector>
#include<iostream>

using namespace std;
int main(int argc, char* argv[])
{
	Lex lex("lex.l", "out.cpp");
	lex.work();
	
	//if (argc == 1) {
	//	cerr << "无lex源文件" << endl;
	//	exit(1);
	//}
	//else if (argc == 2) {
	//	Lex lex(argv[1], "out.cpp");
	//	lex.work();
	//}
	//else {
	//	Lex lex(argv[1], argv[2]);
	//	lex.work();
	//}
	return 0;
}