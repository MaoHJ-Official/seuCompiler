#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
extern string yylex();


int main(int argc,char*argv[]) 
{
ifstream tanaly;
tanaly.open("AnalyTable.txt", ios::in);
int Tablesize;
int Tablefrontsize;
string x;
vector<string>Table;
vector<vector<string>>AnalyTable;
tanaly >> Tablefrontsize >> Tablesize;
if (!tanaly.eof())getline(tanaly, x);
for (int i = 0; i < Tablesize; i++) {
	for (int j = 0; j < Tablefrontsize; j++)
	{
		if (!tanaly.eof())getline(tanaly, x);
		else cout << "Error AnalyTable broken" << endl;
		if (x == " ")x.clear();
		Table.push_back(x);
	}
	AnalyTable.push_back(Table);
	Table.clear();
}
tanaly.close();
string filename;
string para1,para2;
ifstream lex_out;
vector<string> Lex_OUT;
if (argc == 1) {
	yylex();
	filename = "result.txt";
}
else if (argc == 2) {
	para1 = argv[1];
}
else if (argc == 3) {
	para1 = argv[1];
	para2 = argv[2];
}
if (argc == 2 && !para1.compare("-l")) {
	yylex();
	filename = "result.txt";
}
else if (argc == 3 && !para2.compare("-f")) {
	filename = argv[2];
}
lex_out.open(filename, ios::in);
string temp;
do {
lex_out >> temp;
cout << temp << " ";
Lex_OUT.push_back(temp);
} while (!lex_out.eof());
cout<<endl;
Lex_OUT.push_back("$");
stack<int> stateStack;
stack<string> symbolStack;
symbolStack.push("$");
stateStack.push(0);
int cnt = 0;
do {
	for (auto iteral = AnalyTable.begin(); iteral != AnalyTable.end(); ++iteral)
	{
		if (!(*iteral).front().compare(Lex_OUT[cnt]))
		{
			if (!(*iteral)[stateStack.top() + 1].compare("accept"))
			{
				cout << "Successful!!!!!!!!!!!!!!!!" << endl;
				return 0;
				break;
			}
			if (!(*iteral)[stateStack.top() + 1].find("s"))
			{
				string s = (*iteral)[stateStack.top() + 1];
				int i = atoi(s.erase(0, 1).c_str());
				stateStack.push(i);
				symbolStack.push(Lex_OUT[cnt]);
				cout << "移进" << Lex_OUT[cnt] << endl;
				//	cout << "状态" << i << endl;
				break;
			}
			else if ((*iteral)[stateStack.top() + 1].empty())
			{
				cout << "ERROR: syntax errror" << endl;
				exit(1);
			}
			else
			{
				string s1 = (*iteral)[stateStack.top() + 1];
			string s = s1;
				int offset = s.find_first_of("#", 0);
				int i = atoi(s.erase(offset, s.size() - offset).c_str());
				while (i != 0)
				{
					cout << "出栈" << symbolStack.top();
					symbolStack.pop();
					stateStack.pop();
					i--;
				}
				cout << endl;
				//	cout << "当前状态：" << stateStack.top()<<endl;
				string j = s1.erase(0, offset + 1);
				int t = stateStack.top();
				for (auto itera = AnalyTable.begin(); itera != AnalyTable.end(); ++itera)
				{
					if (!(*itera).front().compare(j))
					{
						string temp = (*itera)[t + 1];
						i = atoi(temp.erase(0, 1).c_str());
					stateStack.push(i);
						symbolStack.push(j);
						cout << j << endl;
					}
				}
				cnt--;
			}
			break;
		}
	}
	cnt++;
} while (cnt != Lex_OUT.size());
cout << "Error" << endl;
}
