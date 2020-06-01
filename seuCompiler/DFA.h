#pragma once
#pragma warning(disable:4786)
#ifndef DFA_H
#define DFA_H
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<deque>
#include<stack>
using namespace std;
/***********下面是对边的定义***********/
struct dfaEdge
{
	char symbol; /*边上标记*/
	int nextNode; /*后续状态结点标号*/
	dfaEdge()
	{
		symbol = '\0';
		nextNode = 0;
	}
	dfaEdge(char ch)
	{
		symbol = ch;
		nextNode = 0;
	}
	dfaEdge(const dfaEdge & E)//拷贝构造函数
	{
		symbol = E.symbol;
		nextNode = E.nextNode;
	}
};
/**********边的定义结束**********/

struct dfaNode
{
	int label; /*状态结点标记名*/
	int acceptstatetag; /*接受状态标记，0为非接受状态，1为接受状态*/
	vector<dfaEdge> edges;//存发出边
	map<char, int> hashedges;//存边的快表

	dfaNode(const dfaNode & n)//结点的拷贝构造函数,为了支持vector中相关的插入弹出等操作
	{
		label = n.label;
		acceptstatetag = n.acceptstatetag;
		edges = n.edges;
		hashedges = n.hashedges;
	}
	dfaNode(int lab)
	{
		label = lab;
		acceptstatetag = 0;

	}
	dfaNode()
	{
		label = 0;
		acceptstatetag = 0;
	}
	int nextId(char ch)
	{
		map<char, int>::iterator iter;
		iter = hashedges.find(ch);
		if (iter != hashedges.end())
			return iter->second;
		else
			return -1;
	}

};
struct TerminalNode
{
	int id;//终止结点ID
	string type;//终止结点类型，例如int，id，num。。。。
};
class DFA
{
public:
	DFA();
	void minimizeDfa();//最小化DFA的状态
	set<int> epslonClosure(int s);//求状态s的epslon闭包
	set<int> epslonClosure(set<int> T);//求状态集T的epslon闭包
	set<int> move(set<int> T, char a); //能够从T中某状态出发通过标号为a的转换到达的状态集合
	void subsetCon();//采用子集构造法生成DFA
	void getCharSet();//获得DFA的输入字符集
	void printNodes();//打印DFA结点
	void sortNodes();
	bool isIn(set<int> s, int element);//判断element是否在集合s中
	bool isTerminate(set<int>, vector<int> &terSet, int &);//判断结点集是否含有终止结点
	bool getTerType(int num, string & str);//将终止结点号为num的终止类型赋给str
	friend ostream &operator<<(ostream&, const vector<dfaEdge>);
	void printNode();
	//private:
	set<char> charSet;//DFA的输入字符集
	vector<TerminalNode>  acceptedNode;//存储NFA所有终止结点
	vector<dfaNode>  dfaNodeTable;
	int start;//开始结点
	map<int, string> terminal;
	map<int, int>  isAcc;
};

#endif