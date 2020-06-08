#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<map>
#include<stack>
#include<set>
#include<queue>
using namespace std;

struct Node
{
	Node(int fromId, string input, int toId)
	{
		this->fromId = fromId;
		this->input = input;
		this->toId = toId;
	}
	int fromId;
	string input;
	int toId;
};

class Lex
{
public:
	static const int MAXID = 100000;
	Lex(string _lexFile);
	Lex(string _lexFile, string _outCFile);
	void init();

	//写入scanner之前
	void outCodeLeft();
	//写入scanner之后
	void outCodeRight();
	//写入scanner
	void outCodeMid();
	//解析lex源文件
	void scanner();
	//错误报告
	inline void printError(int line, string str, string mess);
	inline void printWaring(int line, string str, string mess);
	//解析方法体
	void getFunc(string str, int line);
	//解析RE
	void getRegular(string str, int line);
	//RE是否合法
	bool isExpre(vector<pair<string, bool>>);
	//DFA最小化
	void getSimpDFA();
	//添加NFA结点
	inline void addNFANode(int fromId, string input, int toId);
	//添加DFA结点
	inline void addDFANode(int formId, string input, int toId);

	//RE转NFA
	void regrToNFA(vector<pair<string, bool>> regr, int startId, int endId, int& itemId);
	//生成NFA
	void getNFA();
	//生成DFA
	void getDFA();
	//nfa查询
	void findBeGo(int i, string input, vector<int>* state);
	//获取nfa目标状态集合
	vector<int>* getState(vector<int>* fromState, string input);
	//获取终态
	void getVt(map<vector<int>, int> stateMap);

	//执行操作
	void work();

	//输出-测试-
	inline void print();
private:
	string lexFile;
	string outCFile;
	//自定义输入对应方法体
	map<string, string>funcMap;
	//re
	vector<vector<pair<string, bool>>> regrVet;
	//方法体
	vector<string> funcVet;
	//NFA
	vector<Node> nfaVet[MAXID];
	//DFA
	vector<Node>dfaVet[MAXID];
	//储存所有输入
	set<string>inputSet;
	//DFAend
	int dfaEnd[MAXID];
	//NFAend
	int nfaEnd[MAXID];
	int nfaNum;
	int dfaNum;
};