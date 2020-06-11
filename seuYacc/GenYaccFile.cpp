#include"Declaration.h"
extern vector<string> tokenVector;//终结符
extern vector<string> pdnLeft;//非终结符
extern vector<string> functionVector;//第三部分
extern vector<vector<string>> AnalyTable;//LR分析表

void genFile()
{
	//生成 y.tab.h文件
	ofstream out;
	out.open("y.tab.h", ios::out);
	out << "#ifndef Y_TAB_H" << endl;
	out << "#define Y_TAB_H" << endl;

	for (const auto& p : tokenVector)
	{
		out << "#define  " << p << "    " << "\"" << p << "\"" << endl;
	}
	for (const auto& p : pdnLeft)
	{
		out << "#define  " << p << "    " << "\"" << p << "\"" << endl;
	}
	out << "#endif" << endl;
	out.close();

	//生成 y.tab.c文件
	out.open("y.tab.cpp", ios::out);

	if (!out)
	{
		cout << "ERROR: can't open file y.tab.c !" << endl;
	}
	out << "#include <stdio.h>" << endl;
	out << "#include <stdlib.h>" << endl;
	out << "#include <assert.h>" << endl;
	out << "#include <stack>" << endl;
	out << "#include <string>" << endl;
	out << "#include <fstream>" << endl;
	out << "#include <iostream>" << endl;
	out << "#include <vector>" << endl;
	out << "using namespace std;" << endl;
	out << "extern string yylex();" << endl;
	for (const auto& s : functionVector) {
		out << s << endl;
	}
	out << "int main(int argc,char*argv[]) " << endl;
	out << "{" << endl;
	//初始化AnalyTable
	out << "ifstream tanaly;" << endl;
	out << "tanaly.open(\"AnalyTable.txt\", ios::in);" << endl;
	out << "int Tablesize;" << endl;
	out << "int Tablefrontsize;" << endl;
	out << "string x;" << endl;
	out << "vector<string>Table;" << endl;
	out << "vector<vector<string>>AnalyTable;" << endl;
	out << "tanaly >> Tablefrontsize >> Tablesize;" << endl;
	out << "if (!tanaly.eof())getline(tanaly, x);" << endl;
	out << "for (int i = 0; i < Tablesize; i++) {" << endl;
	out << "	for (int j = 0; j < Tablefrontsize; j++)" << endl;
	out << "	{" << endl;
	out << "		if (!tanaly.eof())getline(tanaly, x);" << endl;
	out << "		else cout << \"Error AnalyTable broken\" << endl;" << endl;
	out << "		if (x == \" \")x.clear();" << endl;
	out << "		Table.push_back(x);" << endl;
	out << "	}" << endl;
	out << "	AnalyTable.push_back(Table);" << endl;
	out << "	Table.clear();" << endl;
	out << "}" << endl;
	out << "tanaly.close();" << endl;
	//读lex输出
	out << "string filename;" << endl;
	out << "string para1,para2;" << endl;
	out << "ifstream lex_out;" << endl;
	out << "vector<string> Lex_OUT;" << endl;
	out << "if (argc == 1) {" << endl;
	out << "	yylex();" << endl;
	out << "	filename = \"result.txt\";" << endl;
	out << "}" << endl;
	out << "else if (argc == 2) {" << endl;
	out << "	para1 = argv[1];" << endl;
	out << "}" << endl;
	out << "else if (argc == 3) {" << endl;
	out << "	para1 = argv[1];" << endl;
	out << "	para2 = argv[2];" << endl;
	out << "}" << endl;
	out << "if (argc == 2 && !para1.compare(\"-l\")) {" << endl;
	out << "	yylex();" << endl;
	out << "	filename = \"result.txt\";" << endl;
	out << "}" << endl;
	out << "else if (argc == 3 && !para2.compare(\"-f\")) {" << endl;
	out << "	filename = argv[2];" << endl;
	out << "}" << endl;
	out << "lex_out.open(filename, ios::in);" << endl;
	out << "string temp;" << endl;
	out << "do {" << endl;
	out << "lex_out >> temp;" << endl;
	out << "cout << temp << \" \";" << endl;
	out << "Lex_OUT.push_back(temp);" << endl;
	out << "} while (!lex_out.eof());" << endl;
	out << "cout<<endl;" << endl;
	out << "Lex_OUT.push_back(\"$\");" << endl;
	out << "stack<int> stateStack;" << endl;//状态栈
	out << "stack<string> symbolStack;" << endl;//符号栈
	out << "symbolStack.push(\"$\");" << endl;
	out << "stateStack.push(0);" << endl;//初始化
	out << "int cnt = 0;" << endl;
	out << "do {" << endl;
	out << "	for (auto iteral = AnalyTable.begin(); iteral != AnalyTable.end(); ++iteral)" << endl;
	out << "	{" << endl;
	out << "		if (!(*iteral).front().compare(Lex_OUT[cnt]))" << endl;
	out << "		{" << endl;
	out << "			if (!(*iteral)[stateStack.top() + 1].compare(\"accept\"))" << endl;
	out << "			{" << endl;
	out << "				cout << \"Successful!!!!!!!!!!!!!!!!\" << endl;" << endl;
	out << "				return 0;" << endl;
	out << "				break;" << endl;
	out << "			}" << endl;
	out << "			if (!(*iteral)[stateStack.top() + 1].find(\"s\"))" << endl;
	out << "			{" << endl;
	out << "				string s = (*iteral)[stateStack.top() + 1];" << endl;
	out << "				int i = atoi(s.erase(0, 1).c_str());" << endl;
	out << "				stateStack.push(i);" << endl;
	out << "				symbolStack.push(Lex_OUT[cnt]);" << endl;
	out << "				cout << \"移进\" << Lex_OUT[cnt] << endl;" << endl;
	out << "				//	cout << \"状态\" << i << endl;" << endl;
	out << "				break;" << endl;
	out << "			}" << endl;
	out << "			else if ((*iteral)[stateStack.top() + 1].empty())" << endl;
	out << "			{" << endl;
	out << "				cout << \"ERROR: syntax errror\" << endl;" << endl;
	out << "				exit(1);" << endl;
	out << "			}" << endl;
	out << "			else" << endl;
	out << "			{" << endl;
	out << "				string s1 = (*iteral)[stateStack.top() + 1];" << endl;
	out << "			string s = s1;" << endl;
	out << "				int offset = s.find_first_of(\"#\", 0);" << endl;
	out << "				int i = atoi(s.erase(offset, s.size() - offset).c_str());" << endl;
	out << "				while (i != 0)" << endl;
	out << "				{" << endl;
	out << "					cout << \"出栈\" << symbolStack.top();" << endl;
	out << "					symbolStack.pop();" << endl;
	out << "					stateStack.pop();" << endl;
	out << "					i--;" << endl;
	out << "				}" << endl;
	out << "				cout << endl;" << endl;
	out << "				//	cout << \"当前状态：\" << stateStack.top()<<endl;" << endl;
	out << "				string j = s1.erase(0, offset + 1);" << endl;
	out << "				int t = stateStack.top();" << endl;
	out << "				for (auto itera = AnalyTable.begin(); itera != AnalyTable.end(); ++itera)" << endl;
	out << "				{" << endl;
	out << "					if (!(*itera).front().compare(j))" << endl;
	out << "					{" << endl;
	out << "						string temp = (*itera)[t + 1];" << endl;
	out << "						i = atoi(temp.erase(0, 1).c_str());" << endl;
	out << "					stateStack.push(i);" << endl;
	out << "						symbolStack.push(j);" << endl;
	out << "						cout << j << endl;" << endl;
	out << "					}" << endl;
	out << "				}" << endl;
	out << "				cnt--;" << endl;
	out << "			}" << endl;
	out << "			break;" << endl;
	out << "		}" << endl;
	out << "	}" << endl;
	out << "	cnt++;" << endl;
	out << "} while (cnt != Lex_OUT.size());" << endl;
	out << "cout << \"Error\" << endl;" << endl;
	out << "}" << endl;
}
