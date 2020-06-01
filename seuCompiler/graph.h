#pragma once
#ifndef GRAPH_H
#define GRAPH_H
#define NULL 0

#include<iostream>
using namespace std;
struct Edge { //�߽ڵ㶨��
	int dest;//�ߵ���һ�����λ�ã��������ʾͨ��cost�ߵ���һ������
	char cost;//���ϵ�Ȩֵ,�������ʾ�߼�
	Edge *link; //��һ������ָ��
};


struct Vertex {//����Ķ���
	int data;//���������
	Edge *adj;//�������ͷָ��
	int Isterminal;//�ý���Ƿ�Ϊ��̬
};


class Graph {//ͼ���ඨ��
public:
	Graph(int sz = 0);//���캯��
	~Graph();
	void  insertVertex(const int& vertex);//����һ���ڵ�
	bool  removeVertex(const int& vertex);//ɾ��һ���ڵ�
	void  insertEdge(int v1, int v2, char s);//����һ����
	bool  removeEgde(int v1, int v2);//ɾ��һ����

	//ȡ��һ���ڽӵ����һ���ڽӵ�
//private:	
	int maxVertices;
	int numVertices;
	int numEdges;
	Vertex * NodeTable;//��������������ͷ�ڵ㣩
	int gerVertexPos(const int Vertex)
	{
		for (int i = 0; i < numVertices; i++)
			if (NodeTable[i].data == Vertex) return i;
		return -1;
	}

};





Graph::Graph(int sz)
{
	//���캯��������һ���յ����ӱ�
	maxVertices = sz;
	numVertices = 0;
	//numEdges=0;
	NodeTable = new Vertex[maxVertices];
	//�������������
	if (NodeTable == NULL)
	{
		cerr << "�洢�������" << endl; exit(1);
	}
	for (int i = 0; i < maxVertices; i++)
		NodeTable[i].adj = NULL;
}



Graph::~Graph()
{
	//����������ɾ��һ���ڽӱ�
	for (int i = 0; i < maxVertices; i++)
	{
		Edge*p = NodeTable[i].adj;
		while (p != NULL)
		{
			NodeTable[i].adj = p->link;
			delete p;
			p = NodeTable[i].adj;
		}
	}
	delete[] NodeTable;//ɾ����������
}

void Graph::insertVertex(const int&vertex)
{

	NodeTable[numVertices].data = vertex;
	NodeTable[numVertices].Isterminal = 0;
	numVertices++;
}

void Graph::insertEdge(int v1, int v2, char s)
{
	Edge*p;
	p = new Edge;
	p->cost = s;
	p->dest = v2;
	p->link = NodeTable[v1].adj;
	NodeTable[v1].adj = p;

}
#endif