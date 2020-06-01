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

/*******������ʹ�õ����ݽṹ*******/
struct node
{
	/***********�����ǶԱߵĶ���***********/
	struct Edge
	{
		char symbol; /*���ϱ��*/
		struct node *nextnode; /*����״̬���*/
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
		Edge(const Edge & E)//�������캯��
		{
			symbol = E.symbol;
			nextnode = E.nextnode;
		}
	};
	/**********�ߵĶ������**********/


	int label; /*״̬�������*/
	int acceptstatetag; /*����״̬��ǣ�0Ϊ�ǽ���״̬��1Ϊ����״̬*/
	Edge firstEdge, lastEdge;//������ɭ�㷨֪��ÿ����������������


	node(const node & n)//���Ŀ������캯��,Ϊ��֧��vector����صĲ��뵯���Ȳ���
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
	int id;//��ֹ���ID
	string type;//��ֹ������ͣ�����int��id��num��������
};*/
/*******���ݽṹ�������*******/
class NFA
{
public:
	NFA();//���캯��
	NFA(const NFA& nfa);
	NFA * reToNFA(string& re);//������������ʽreת��ΪNFA
	int priority(string c);
	void error();
	void change(string & r1, string &r2);//����׺���ʽת��Ϊ��׺���ʽ
	bool isletter(char c);//�ж�c�Ƿ�Ϊ��ĸ����Ԫ��
	bool isletter(string & c);
	void asign(NFA & nfa); //��������ֵ��nfa
	NFA& operator= (const NFA& right);
	void preProcess(string & str);//��������ʽԤ��������.���ӷ�
	void convrtAll(map<string, string> &reTable, map<string, string> &actionTable);//�����ڱ�reTable�е�����������ʽװ��Ϊһ����NFA
	bool isIn(map<string, string> reTable, string str);//
	void join(NFA & nfa);//����ǰ��NFA��nfa�ϲ�
	char strToChar(string & str);
	void printTer();
	void preOrder(node *ptr, DFA& dfa);
	void nfaTodfa(DFA & dfa);
	void getCharSet(map<string, string>reTable, map<string, string>reaction);//���NFA�������ַ���
private:
	//������ɭ�㷨�������NFAֻ��һ����ʼ״̬��һ������״̬
	node * start;           //ָ��NFA�Ŀ�ʼ���
	node * end;             //ָ��NFA����ֹ���
	int  nodenum;
	vector<node*> nfaNodes;  //�洢NFA�����н��
	vector<TerminalNode>  terminalNode;//�洢NFA������ֹ���
	set<char> charSet;//�����ַ���

};

#endif