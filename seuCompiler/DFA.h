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
/***********�����ǶԱߵĶ���***********/
struct dfaEdge
{
	char symbol; /*���ϱ��*/
	int nextNode; /*����״̬�����*/
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
	dfaEdge(const dfaEdge & E)//�������캯��
	{
		symbol = E.symbol;
		nextNode = E.nextNode;
	}
};
/**********�ߵĶ������**********/

struct dfaNode
{
	int label; /*״̬�������*/
	int acceptstatetag; /*����״̬��ǣ�0Ϊ�ǽ���״̬��1Ϊ����״̬*/
	vector<dfaEdge> edges;//�淢����
	map<char, int> hashedges;//��ߵĿ��

	dfaNode(const dfaNode & n)//���Ŀ������캯��,Ϊ��֧��vector����صĲ��뵯���Ȳ���
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
	int id;//��ֹ���ID
	string type;//��ֹ������ͣ�����int��id��num��������
};
class DFA
{
public:
	DFA();
	void minimizeDfa();//��С��DFA��״̬
	set<int> epslonClosure(int s);//��״̬s��epslon�հ�
	set<int> epslonClosure(set<int> T);//��״̬��T��epslon�հ�
	set<int> move(set<int> T, char a); //�ܹ���T��ĳ״̬����ͨ�����Ϊa��ת�������״̬����
	void subsetCon();//�����Ӽ����취����DFA
	void getCharSet();//���DFA�������ַ���
	void printNodes();//��ӡDFA���
	void sortNodes();
	bool isIn(set<int> s, int element);//�ж�element�Ƿ��ڼ���s��
	bool isTerminate(set<int>, vector<int> &terSet, int &);//�жϽ�㼯�Ƿ�����ֹ���
	bool getTerType(int num, string & str);//����ֹ����Ϊnum����ֹ���͸���str
	friend ostream &operator<<(ostream&, const vector<dfaEdge>);
	void printNode();
	//private:
	set<char> charSet;//DFA�������ַ���
	vector<TerminalNode>  acceptedNode;//�洢NFA������ֹ���
	vector<dfaNode>  dfaNodeTable;
	int start;//��ʼ���
	map<int, string> terminal;
	map<int, int>  isAcc;
};

#endif