#pragma once
#ifndef GRAPH_H
#define GRAPH_H
#define NULL 0

#include<iostream>
using namespace std;
struct Edge { //边节点定义
	int dest;//边的另一顶点的位置，在这里表示通过cost边的下一个顶点
	char cost;//边上的权值,在这里表示边集
	Edge *link; //下一条边链指针
};


struct Vertex {//顶点的定义
	int data;//顶点的名字
	Edge *adj;//边链表的头指针
	int Isterminal;//该结点是否为终态
};


class Graph {//图的类定义
public:
	Graph(int sz = 0);//构造函数
	~Graph();
	void  insertVertex(const int& vertex);//插入一个节点
	bool  removeVertex(const int& vertex);//删除一个节点
	void  insertEdge(int v1, int v2, char s);//插入一条边
	bool  removeEgde(int v1, int v2);//删除一条边

	//取第一个邻接点和下一个邻接点
//private:	
	int maxVertices;
	int numVertices;
	int numEdges;
	Vertex * NodeTable;//顶点表（各边链表的头节点）
	int gerVertexPos(const int Vertex)
	{
		for (int i = 0; i < numVertices; i++)
			if (NodeTable[i].data == Vertex) return i;
		return -1;
	}

};





Graph::Graph(int sz)
{
	//构造函数：建立一个空的链接表
	maxVertices = sz;
	numVertices = 0;
	//numEdges=0;
	NodeTable = new Vertex[maxVertices];
	//创建顶点数组表
	if (NodeTable == NULL)
	{
		cerr << "存储分配错误！" << endl; exit(1);
	}
	for (int i = 0; i < maxVertices; i++)
		NodeTable[i].adj = NULL;
}



Graph::~Graph()
{
	//析构函数，删除一个邻接表
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
	delete[] NodeTable;//删除顶点数组
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