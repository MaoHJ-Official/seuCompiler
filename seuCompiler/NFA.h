#pragma once
#ifndef NFA_H
#define NFA_H
#include<string>
#include<map>
#include<vector>
#include<deque>
#include<list>
#include<iostream>
using namespace std;
#include"DFA.h"

/*******定义所使用的数据结构*******/
struct node
{
	/***********下面是对边的定义***********/
	struct Edge
	{
		char symbol; /*边上标记*/
		struct node *nextnode; /*后续状态结点*/
		Edge()
		{
			symbol = '\0';
			nextnode = NULL;
		}
		Edge(char ch)
		{
			symbol = ch;
			nextnode = NULL;
		}
		Edge(const Edge & E)//拷贝构造函数
		{
			symbol = E.symbol;
			nextnode = E.nextnode;
		}
	};
	/**********边的定义结束**********/


	int label; /*状态结点标记名*/
	int acceptstatetag; /*接受状态标记，0为非接受状态，1为接受状态*/
	Edge firstEdge, lastEdge;//由托马森算法知：每个结点最多有两条边


	node(const node & n)//结点的拷贝构造函数,为了支持vector中相关的插入弹出等操作
	{
		label = n.label;
		acceptstatetag = n.acceptstatetag;
		firstEdge = n.firstEdge;
		lastEdge = n.lastEdge;

	}
	node(int lab)
	{
		label = lab;
		acceptstatetag = 0;

	}
	node()
	{
		label = 0;
		acceptstatetag = 0;
	}
};

/*
struct TerminalNode
{
	int id;//终止结点ID
	string type;//终止结点类型，例如int，id，num。。。。
};*/
/*******数据结构定义结束*******/
class NFA
{
public:
	NFA();//构造函数
	NFA(const NFA& nfa);
	NFA * reToNFA(string& re);//将单个正规表达式re转换为NFA
	int priority(string c);
	void error();
	void change(string & r1, string &r2);//将中缀表达式转换为后缀表达式
	bool isletter(char c);//判断c是否为字母表中元素
	bool isletter(string & c);
	void asign(NFA & nfa); //将本对象赋值给nfa
	NFA& operator= (const NFA& right);
	void preProcess(string & str);//将正规表达式预处理：插入.连接符
	void convrtAll(map<string, string> &reTable, map<string, string> &actionTable);//将存在表reTable中的所有正规表达式装换为一个大NFA
	bool isIn(map<string, string> reTable, string str);//
	void join(NFA & nfa);//将当前的NFA和nfa合并
	char strToChar(string & str);
	void printTer();
	void preOrder(node *ptr, DFA& dfa);
	void nfaTodfa(DFA & dfa);
	void getCharSet(map<string, string>reTable, map<string, string>reaction);//获得NFA的输入字符集
private:
	//由托马森算法构造出的NFA只有一个开始状态和一个结束状态
	node * start;           //指向NFA的开始结点
	node * end;             //指向NFA的终止结点
	int  nodenum;
	vector<node*> nfaNodes;  //存储NFA的所有结点
	vector<TerminalNode>  terminalNode;//存储NFA所有终止结点
	set<char> charSet;//输入字符集

};

#endif