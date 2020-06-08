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

	//д��scanner֮ǰ
	void outCodeLeft();
	//д��scanner֮��
	void outCodeRight();
	//д��scanner
	void outCodeMid();
	//����lexԴ�ļ�
	void scanner();
	//���󱨸�
	inline void printError(int line, string str, string mess);
	inline void printWaring(int line, string str, string mess);
	//����������
	void getFunc(string str, int line);
	//����RE
	void getRegular(string str, int line);
	//RE�Ƿ�Ϸ�
	bool isExpre(vector<pair<string, bool>>);
	//DFA��С��
	void getSimpDFA();
	//���NFA���
	inline void addNFANode(int fromId, string input, int toId);
	//���DFA���
	inline void addDFANode(int formId, string input, int toId);

	//REתNFA
	void regrToNFA(vector<pair<string, bool>> regr, int startId, int endId, int& itemId);
	//����NFA
	void getNFA();
	//����DFA
	void getDFA();
	//nfa��ѯ
	void findBeGo(int i, string input, vector<int>* state);
	//��ȡnfaĿ��״̬����
	vector<int>* getState(vector<int>* fromState, string input);
	//��ȡ��̬
	void getVt(map<vector<int>, int> stateMap);

	//ִ�в���
	void work();

	//���-����-
	inline void print();
private:
	string lexFile;
	string outCFile;
	//�Զ��������Ӧ������
	map<string, string>funcMap;
	//re
	vector<vector<pair<string, bool>>> regrVet;
	//������
	vector<string> funcVet;
	//NFA
	vector<Node> nfaVet[MAXID];
	//DFA
	vector<Node>dfaVet[MAXID];
	//������������
	set<string>inputSet;
	//DFAend
	int dfaEnd[MAXID];
	//NFAend
	int nfaEnd[MAXID];
	int nfaNum;
	int dfaNum;
};