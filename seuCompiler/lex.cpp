#pragma warning(disable:4786)
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<stdio.h>
#include<string>
#include<map>
#include<vector>
#include"NFA.h"
using namespace std;
#define ERROR  -1;
#define BEGIN   1;
#define END     2;
#define SEGMENT 3;
ifstream ifile;
ofstream ofile;
int lines = 0;
map<string, string>reTable;
map<string, string>reaction;
vector<string> actionTable;
struct RE {
	string one;
	string two;
	struct *next;
}RE;

int check(char ch)//判断%下一个字符串是什么
{
	if (ch == '%')
	{
		char nextch = ifile.get();
		switch (nextch)
		{
		case '{':return BEGIN;
		case '}':return END;
		case '%':return SEGMENT;
		default:ifile.seekg(-1, ios::cur);
			break;
		}
	}
	return ERROR;
}
bool IsLetterNum(char ch)//判断规则部分字符串是否符合规则
{
	if ((ch <= '9'&&ch >= '0') || (ch <= 'z'&&ch >= 'a') || (ch <= 'Z'&&ch >= 'A'))
	{
		return true;
	}
	return false;
}

bool compleRe(string &re)//把规则部分转化成正则表达式
{
	int intcount;
	string str = "";
	string temp;
	map<string, string>::iterator iter;
	int i = 0;
	int j;
	int offset;
	char ch = re[i];
	while (ch != '\0')
	{
		switch (ch)
		{
		case'[':
			str.append(1, '(');
			ch = re[++i];
			continue;
			break;
		case']':
			str.append(1, ')');
			ch = re[++i];
			break;
		case'-':
		{
			char before = re[i - 1];
			char after = re[i + 1];
			str.erase(str.length() - 1, 1);
			if (IsLetterNum(re[i - 2]))
			{
				str.append(1, '|');
			}
			while (before < after)
			{
				str.append(1, before);
				str.append(1, '|');
				before++;
			}
			str.append(1, after);
			ch = re[i + 2];
			i = i + 2;
			break;
		}
		case'{':
			offset = re.find_first_of('}', i);
			for (j = i + 1; j < offset; j++)
			{
				temp.append(1, re[j]);
			}
			iter = reTable.find(temp);
			intcount = reTable.count(temp);
			if (intcount <= 0)
			{
				return false;
			}
			if (iter != reTable.end())
			{
				str.append(1, '(');
				str.append(iter->second);
				str.append(1, ')');
			}
			temp = "";
			i = offset;
			ch = re[++i];
			break;
		case'+':
			str.append(1, re[i - 1]);
			str.append(1, '*');
			ch = re[++i];
		case'|':
			str.append(1, '|');
			ch = re[++i];
			break;
		case'"':
			offset = re.find_first_of('"', i + 1);
			temp = re.substr(i + 1, offset - i - 1);
			str.append(1, '(');
			str.append(temp);
			str.append(1, ')');
			i = offset;
			ch = re[++i];
			break;
		default:
			str.append(1, ch);
			ch = re[++i];
			break;
		}
	}
	re = str;
	return true;
}

void GenerateCode(DFA & dfa);
int main()
{
	NFA nfa1;
	ofile.open("yylex.cpp", ios::out);
	ifile.open("test.txt", ios::out);
	if (!ifile)
	{
		cerr << "File could not be opened" << endl;
		exit(1);
	}
	char ch;
	ch = ifile.get();
	int state;
	state = check(ch);
	if (state != 1)
	{
		cerr << "The formation of the inputfile is wrong" << endl;
		exit(1);
	}
	while (!ifile.eof() && state != 2)
	{
		char nextch = ifile.get();
		//if(nextch=='\t'||nextch==' ')
			//continue;
		if (nextch == '%')
		{
			state = check(nextch);
			continue;
		}
		if (nextch == '\n')
		{
			char nextch = ifile.get();
			if (nextch == '\n')
			{
				ofile.put('\n');
				continue;
			}
			else
			{
				ifile.seekg(-1, ios::cur);
			}

		}
		ofile.put(nextch);
	}
	cout << "The definition is finished" << endl;
	//以上部分把定义部分输入到文件中
	cout << "The rule RE expression begin" << endl;
	ifile.get();
	pair<string, string>pire;
	state = BEGIN;
	string id, re;
	while (!ifile.eof() && state != 3)
	{
		char nextch = ifile.get();
		if (nextch == '\n')
		{
			int k = 0;
			char nextch = ifile.get();
			while (nextch == '\n')
			{
				nextch = ifile.get();
				continue;
			}
			ifile.seekg(-1, ios::cur);

		}
		if (nextch == '%')
		{
			state = check(nextch);
			if (state == -1)
			{
				cerr << "There is a mistake" << endl;
			}
			continue;
		}
		else
		{
			ifile.seekg(-1, ios::cur);
			ifile >> id >> re;
			compleRe(re);
			RE.one = id;
			RE.two = re;
		}
		//string id,re;
		//ifile>>id>>re;
		/*pire.first=id;
		compleRe(re);
		pire.second=re;
		reTable.insert(pire);*/
		reTable.insert(pair<string, string>(RE.one, RE.two));//把正则表达式存在map中
		lines++;
		ifile.get();
		//执行对正规表达式的分析
	}
	map<string, string>::iterator iter;
	for (iter = reTable.begin(); iter != reTable.end(); iter++)
	{
		cout << iter->first << " " << iter->second << endl;
	}
	cout << "The rule RE expression finished" << endl;
	cout << "The keyword segment begin" << endl;
	ifile.get();
	state = BEGIN;
	pair<string, string>pi;
	while (!ifile.eof() && state != 3)
	{
		char nextch1 = ifile.get();
		if (nextch1 == '\n')
		{
			char nextch1 = ifile.get();
			if (nextch1 == '\n')
			{
				//ofile.put('\n');
				continue;
			}
			else
			{
				ifile.seekg(-1, ios::cur);
			}

		}
		if (nextch1 == '%')
		{
			state = check(nextch1);
			if (state == -1)
			{
				cerr << "There is a mistake" << endl;
			}
			continue;
		}
		else
		{
			ifile.seekg(-1, ios::cur);
		}
		string str;
		string re, action;
		getline(ifile, str);
		string delim = " \t";
		size_t offset = str.find_first_of(delim);
		re = str.substr(0, offset);
		while (str[offset] == ' ' || str[offset] == '\t') offset++;
		action = str.substr(offset, str.size() - offset);
		pi.first = re;
		pi.second = action;
		reaction.insert(pi);
		actionTable.push_back(action);//规则部分把模式和动作分别存储在reaction中，first存re，second存action
	}
	//map<string,string>::iterator iter;
	for (iter = reaction.begin(); iter != reaction.end(); iter++)
	{
		cout << iter->first << " " << iter->second << endl;
	}//对re和action的读取

	nfa1.convrtAll(reTable, reaction);//将存储于reaction表中的正规表达式转换为一个大的NFA
	DFA dfa;
	nfa1.nfaTodfa(dfa);
	dfa.subsetCon();
	dfa.minimizeDfa();

	GenerateCode(dfa);


	cout << "The keyword segment end" << endl;
	ifile.get();
	while (!ifile.eof())
	{
		char nextch = ifile.get();
		if (nextch != EOF)
			ofile.put(nextch);
	}
	ifile.close();
	ofile.close();
	//cout<<nfa1.isIn(reTable,id)<<endl;
	return 0;
}

void GenerateCode(DFA& dfa)//to generate code according to DFA
{
	//对语言的编写要求是个识别的表达式之间要加空白符

	ofile << "using namespace std;" << endl;
	//ofile<<"const int START="<<0<<";"<<endl;      
	ofile << "const int MYERROR=65537;" << endl;
	ofile << endl;

	//yytext is the string needed to be recognized,n is the total states of nfa
	ofile << "int analysis(string yytext)\n";
	ofile << "{\n";///////////////////{
	//ofile<<"\tint state="<<dfa.dfaNodeTable[0].label<<";\n";

	ofile << "\tint state=" << dfa.start << ";\n";


	ofile << "\tint i=0;\n";
	//ofile<<"\tint j="<<dfa.dfaNodeTable[0].label<<";\n";
	ofile << "\tchar ch=yytext[i];\n";

	ofile << "\twhile(i<=yytext.length())\n";
	ofile << "\t{\n";//////////////////{
	ofile << "\t\tswitch(state)\n";
	ofile << "\t\t{\n";//////////////////{
	int len = dfa.dfaNodeTable.size();
	int EROR = dfa.start + len + 1;
	for (int i = 0; i < dfa.dfaNodeTable.size(); i++)   //对每一个dfa状态都有一个case
	{
		int flagofelseif = 0;
		dfaNode curNode = dfa.dfaNodeTable[i];
		int curState = curNode.label;
		ofile << "\t\tcase " << curState << ":\n";

		ofile << "\t\t\t{\n";//////////////////////{

		if (curNode.acceptstatetag == 1)//该状态是终止状态
		{
			ofile << "\t\t\t\tif(i==yytext.length())\n";//已经识别完成打印出相应得动作
			ofile << "\t\t\t\t{\n";
			ofile << "\t\t\t\t";
			/////////////这一段代码是为了获得DFA对应动作的代码
			string str;
			dfa.getTerType(curState, str);
			//cout<<str<<endl;
			string statement = reaction[str];
			int size = statement.size();
			str = statement.substr(1, size - 2);
			////////////
			ofile << '\t' << str << '\n';

			ofile << "\t\t\t\t\tbreak;\n";
			ofile << "\t\t\t\t}\n";

		}
		for (int k = 0; k < curNode.edges.size(); k++)//逐个扫描该结点的边
		{

			ofile << "\t\t\t\t";//输出对边上字母判断的if..else.. 语句
			if (k != 0)
				ofile << "else ";
			ofile << "if(ch=='" << curNode.edges[k].symbol << "')" << '\n';
			ofile << "\t\t\t\t{\n";
			ofile << "\t\t\t\t\tstate=" << curNode.edges[k].nextNode << ";\n";
			ofile << "\t\t\t\t\tbreak;\n";
			ofile << "\t\t\t\t}\n";

		}
		ofile << "\t\t\t\telse\n";//遇到边上没有的字母，直接报错
		ofile << "\t\t\t\t{\n";
		ofile << "\t\t\t\t\tstate=" << EROR << ";\n";
		ofile << "\t\t\t\t\tbreak;\n";
		ofile << "\t\t\t\t}\n";


		ofile << "\t\t\t}\n";
	}
	ofile << "\t\tcase " << EROR << ":\n";
	ofile << "\t\t\t\treturn  MYERROR ;\n";
	ofile << "\t\t}\n";
	ofile << "\t\tch=yytext[++i];  //ch is the next letter to be recongnized\n";
	ofile << "\t}\n";
	ofile << "\treturn  MYERROR ;\n";
	ofile << "\n}\n";
}
