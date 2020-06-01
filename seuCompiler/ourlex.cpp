#include"graph.h"
#include<iostream>
#include<set>
#include<string>
#include<fstream>
#include<stack>
#include<vector>
using namespace std;


////定义全局变量////
ifstream infile;
ofstream outfile;


void remove(int i, int t);
int getSI(vector<set<int>> temp, int n);//判断标号在哪个集合下
int getID(int label, vector< set<int> > vecset, int n);
set<int> closed(set<int> temp);
char getEdge(set<char> edge, int n);
bool equal(vector< set<int> > vecset, set<int> temp);
struct node
{
	int label;//标记状态号
	bool Isterminal;//标记是否为终结状态
};
//有穷状态集s,字母表用一个set集合表示,接受状态集f,转化函数即如何确定后继结点
//生成的边集是固定不变的
set<char> s;//边集合
Graph dfaTable;
Graph nfaTable;
Graph lastTable;
//nfa和dfa的区别，他们通过边得到的结果数量是不一样的，nfa是不定的，dfa是唯一的，随之节点如何通过边得到结果是重点,构造邻接表可以解决图的表示问题






//NFA转换成DFA
void NFAtoDFA()
{
	//从nfaTable图中读结点，每次构造新的dfa结点
	vector<set<int>> nfa1(100);
	nfa1[0].insert(0);//s0赋给I0
	set<int> current = nfa1[0];
	nfa1[0] = closed(current);
	current.clear();//求s0的闭包
	dfaTable.NodeTable[0].data = 0;
	dfaTable.NodeTable[0].adj = NULL;
	int NumberOfI = 0;//确定话后集合I的数量

	//从I中出发的终止条件temp与前面的一样，不再有新的集合出现
	bool flag = true;
	bool symbol = false;
	int i = 0;
	while (flag)
	{
		//开始遍历以num开头的链表，找出在不同边下的集合。（边在集合s中）
		for (int edge = 0; edge < s.size(); edge++)//遍历整个边集
		{
			//读状态集I出发边形成的下个状态集
			for (int j = 0; j < nfa1[i].size(); j++)//I状态中所有的结点
			{
				int num = getID(j, nfa1, i);
				Vertex * node;
				Edge *p;
				node = &nfaTable.NodeTable[num];
				p = node->adj;
				set<int> temp;//临时存放某条边下的集合，判断是否与前面的集合相一致yu nfa1[i].中的元素相比是否相同，不同则创建新的集合，同则不创建
				while (p != NULL)//遍历整个出边表
				{
					if (p->cost == getEdge(s, edge))
					{
						temp.insert(p->dest);
						if (nfaTable.NodeTable[p->dest].Isterminal == 1) symbol = true;
					}
					p = p->link;
				}
				if (temp.size() > 0)
					temp = closed(temp);
				//生成的集合求elpsion闭包
				//判断该集合值temp是否与前面的已有的集合相同
				if (equal(nfa1, temp) && temp.size() > 0)
				{
					//在原有结点下插入边链表
					char tempedge = getEdge(s, edge);
					dfaTable.insertEdge(i, i, tempedge);
				}
				else if (temp.size() > 0)//temp大小必须大于0，否则为空集，不加入出边表中
				{
					//dfa中集合数量加一,nfa1中也增加了一个新的变量
					NumberOfI++;
					nfa1[NumberOfI].insert(temp.begin(), temp.end());
					dfaTable.insertVertex(NumberOfI);
					if (symbol)  dfaTable.NodeTable[NumberOfI].Isterminal = 1;
					dfaTable.insertEdge(i, NumberOfI, getEdge(s, edge));
				}
				temp.clear();
			}
		}
		i++;//i表示你所在执行的结点，numberofI表示新生成的结点（一个i的循环中可以最多生成 s.size()个新的集合）
		if (nfa1[i].size() <= 0) flag = false;
	}
}

set<int> closed(set<int> temp)
{
	//用栈保存得到的结点
	stack<int> tempnode;
	for (int i = 0; i < temp.size(); i++)
	{
		set<int>::iterator  current;
		current = temp.find(i);
		Vertex * node;
		Edge *p;
		node = &nfaTable.NodeTable[*current];
		p = node->adj;
		while (p != NULL)
		{
			if (p->cost == '@')
			{
				temp.insert(p->dest);
				tempnode.push(p->dest);
			}
			p = p->link;
		}

	}
	while (!tempnode.empty())
	{
		int k = tempnode.top();
		tempnode.pop();
		Vertex * node;
		Edge *p;
		node = &nfaTable.NodeTable[k];
		p = node->adj;
		while (p != NULL)
		{
			if (p->cost == '@')
			{
				temp.insert(p->dest);
				tempnode.push(p->dest);
			}
			p = p->link;
		}
	}
	return temp;
}

int getID(int label, vector< set<int> > vecset, int n)
{
	int error = -1;
	set<int> current;
	current = vecset[n];
	int i = 0;
	set<int>::iterator it;
	for (it = current.begin(); it != current.end(); it++)
	{

		if (i == label) return *it;
		i++;
	}
	return error;  // 没有找到
}


char getEdge(set<char> edge, int n)//获得边的集合中相应位置的边
{
	/*set<char>::iterator current;
	current=edge.find(n);
	return *current;*/
	int i = 0;
	set<char>::iterator current;
	for (current = edge.begin(); current != edge.end(); current++)
	{

		if (i == n) return *current;
		i++;
	}
	return '?';
}

bool equal(vector< set<int> > vecset, set<int> temp)//判断新的集合是否与前面的集合重复
{

	bool flag = false;
	for (int i = 0; i < vecset.size(); i++)
	{
		if (vecset[i] == temp)  return true;
	}
	return false;
}
vector<set<int>> minidfa(100);
//最小化DFA;
void miniDFA()
{
	//先根据终态和非终态两种情况分离成两种子集
	set<int> f;
	set<int> ds;
	for (int i = 0; i < dfaTable.numVertices; i++)
	{
		if (dfaTable.NodeTable[i].Isterminal == 0)
			ds.insert(dfaTable.NodeTable[i].data);
		else
			f.insert(dfaTable.NodeTable[i].data);
	}

	int lastI = 1;
	//对于两个集进行循环比较直至最后集合都不变，或者只剩一个。先分集再生成边
	bool flag = true;
	bool symbol = true;
	if (ds.size() == 0)
	{
		flag = false;
	}
	else
	{
		minidfa[0].insert(ds.begin(), ds.end());
		minidfa[1].insert(f.begin(), f.end());
		lastI = 2;
	}
	stack<int> newnode;
	stack<int> nextS;
	stack<int> itself;
	stack<int> protect;//保存集合I中状态新生成集合的节点所对应的状态
	stack<int> news;
	int symbolsize = minidfa.size();
	while (flag)
	{
		int flagtemp = minidfa.size();
		for (int edge = 0; edge < s.size(); edge++)//遍历整个边集
		{
			char tempchar = getEdge(s, edge);
			while (symbol)
			{
				//新增的结点保存：minidfa增加结点即可
				//新增动态的结点：
				//如何判断相等与否：循环minidfa
				//循环所要判断的集合，对集合中每个状态通过边生成的后继状态，保存其所在集合，下次再判断下一个状态生成的集合，若不相同，则继续向
				//其中压入栈中，查看栈的大小，在进行比较，若栈的大小大于一，则将原来的集合分裂，我们只需知道最后分裂结束的集合即可。
				//俩个栈，一个存放对应集合下各个结点的标号，另一个存放这些结点在该边下对应的集合标号

				int tempsize = minidfa.size();

				for (int i = 0; i < tempsize; i++)
				{
					for (int j = 0; j < minidfa[i].size(); j++)
					{
						int temp = getID(j, minidfa, i);
						itself.push(temp);
						Vertex * node;
						Edge *p;
						node = &nfaTable.NodeTable[temp];
						p = node->adj;
						bool iskongji = true;
						while (p != NULL)
						{
							if (p->cost == tempchar)
							{
								//判断p->dest在哪个集合
								int SI = getSI(minidfa, p->dest);
								nextS.push(SI);
								iskongji = false;
							}
							p = p->link;
						}
						if (iskongji) nextS.push(i);
					}
					//遍历俩个栈，若下一个集合栈的栈顶元素不是i,则新建一个集合，将itself栈顶的元素加入，顺便再删除原集合中的元素
					stack<int> tempstack;
					stack<int> tempnew;
					while (!nextS.empty())
					{
						int t = nextS.top();
						nextS.pop();
						int t1 = itself.top();
						itself.pop();
						if (t != i)//如果t不是i,则需要判断t是否和前面出来的状态转向同一个集合，若不是则需要增加一个新的结点，是则添加至原有的那个结点
						{

							bool find = true;
							while (find && !protect.empty())
							{
								int a = protect.top();
								protect.pop();
								tempstack.push(a);
								int b = news.top();
								news.pop();
								tempnew.push(b);
								if (t == a)
								{
									minidfa[b].insert(t1);
									remove(i, t1);
								}
							}
							while (!tempstack.empty())
							{
								protect.push(tempstack.top());
								tempstack.pop();
								news.push(tempnew.top());
								news.pop();
							}
							if (find == false)
							{
								lastI++;
								protect.push(t);
								news.push(lastI);
							}
						}

					}

				}
				symbolsize = minidfa.size();
				if (symbolsize == tempsize)
					symbol = false;

			}
		}
		int  flagsize = minidfa.size();
		if (flagsize == flagtemp) flag = false;

	}
	//对符号集中的每一条边进行判断
	//生成最后的最小化的dfa表
	//添加顶点
	//int nodenumber=0;
//	stack<int> tempnode;
	//stack<int> tempid;
	for (int i = 0; i < minidfa.size(); i++)
	{
		if (minidfa[i].size() > 0)
		{
			lastTable.insertVertex(i);
			//  nodenumber++;
			//  tempnode.push(i);
			 // tempid.push(i);
			for (int j = 0; j < minidfa[i].size(); j++)
			{
				int k = getID(j, minidfa, i);
				if (dfaTable.NodeTable[k].Isterminal == 1)
				{
					lastTable.NodeTable[i].Isterminal = 1;
					break;
				}
			}
		}
	}
	//添加边
	for (int i = lastTable.numVertices; i > 0; i--)
	{
		//	int j=tempnode.top();
		//	tempnode.pop();
		int j = lastTable.NodeTable[i].data;
		lastTable.NodeTable[i].adj = NULL;
		//	lastTable.NodeTable[i].data=i;

		for (int z = 0; z < minidfa[j].size(); z++)//对集合下所有状态进行判断
		{
			int num = getID(z, minidfa, j);
			//对于其每个状态的出边表中的下一个状态在哪个集合中，就将这俩个集合对应的节点通过该边联系起来
			Edge *p;
			p = dfaTable.NodeTable[num].adj;
			while (p != NULL)
			{
				int id = getSI(minidfa, p->dest);
				int nex;
				//根据id,将id对应的结点找出来
				for (int c = 0; c <= lastTable.numVertices; c++)
				{
					if (id == lastTable.NodeTable[c].data)
					{
						nex = c;
						break;
					}
				}
				lastTable.insertEdge(i, nex, p->cost);
				p = p->link;
			}

		}
	}

}

int getSI(vector<set<int>> temp, int n)//判断标号在哪个集合下
{
	set<int> ::iterator current;
	for (int i = 0; i < temp.size(); i++)
	{
		for (current = temp[i].begin(); current != temp[i].end(); current++)
			if (n == *current)  return i;
	}
	return -1;
}

void remove(int i, int t)
{
	set<int>::iterator current;
	current = minidfa[i].find(t);
	minidfa[i].erase(current);

}


void GenerateCode(vector<node> dfanode)//to generate code according to DFA
{
	//对语言的编写要求是个识别的表达式之间要加空白符

	outfile << "using namespace std;" << endl;
	outfile << "const int START=" << 0 << ";" << endl;
	outfile << "const int MYERROR=65537;" << endl;
	outfile << endl;
	//yytext is the string needed to be recognized,n is the total states of nfa
	outfile << "string analysis(string yytext)\n";
	outfile << "{\n";///////////////////{
	outfile << "\tint state=START;\n";
	outfile << "\tint i=0;\n";
	outfile << "\tchar ch=yytext[i];\n";
	//outfile<<"\tint N=n+1 //N表示串长加1，为与状态数保持一致\n";
	outfile << "\twhile(i<=yytext.length())\n";
	outfile << "\t{\n";//////////////////{
	outfile << "\t\tswitch(state)\n";
	outfile << "\t\t{\n";//////////////////{
	for (int i = 0; i < lastTable.numVertices; i++)   //对每一个dfa状态都有一个case
	{
		int flagofelseif = 0;
		outfile << "\t\tcase " << i << ":\n";

		outfile << "\t\t\t{\n";//////////////////////{
		if (lastTable.NodeTable[i].Isterminal == 1)
		{
			outfile << "\t\t\t\tif(i==yytext.length())\n";//已经识别完成打印出相应得动作
			outfile << "\t\t\t\t{\n";///////////////////{
		//	map<int,string>::iterator it;
			//it=TerStateActionTable.find(dfanode[i].GetState());
		//	it=mindfareturn.find(dfanode[i].GetState());
			//if (it!=TerStateActionTable.end())
		//	if (it!=mindfareturn.end())
			//{
			//	int length=((*it).second).length();
			//	outfile<<"\t\t\t\t\t"<<((*it).second).substr(1,length-2)<<endl;
			outfile << "\t\t\t\t\t";
			Edge * endp;
			endp = lastTable.NodeTable[i].adj;
			// string a;
			while (endp != NULL)
			{
				char end;
				if (endp->dest == i)
					end = endp->cost;
				cout << end;

				endp = endp->link;
			}
			cout << endl;
			outfile << "\t\t\t\t\tbreak;\n";
			outfile << "\t\t\t\t}\n";///////////////////if(i==yytext.length){---}
		//}

		}

		//cout<<"hanshu  "<<mymap.size()<<endl;
		Edge *p;
		p = lastTable.NodeTable[i].adj;
		while (p != NULL)
		{
			outfile << "\t\t\t\t";
			if (flagofelseif == 0)
			{
				outfile << "if";
				flagofelseif++;
			}
			else
			{
				outfile << "else if";
			}
			outfile << "(ch=='" << p->cost << "')\n";
			outfile << "\t\t\t\t{\n";////////////if(ch='*'){
			outfile << "\t\t\t\t\tstate=" << p->dest << ";\n";
			outfile << "\t\t\t\t\tbreak;\n";
			outfile << "\t\t\t\t}\n";//////if(ch='*'){----}
			p = p->link;

		}
		if (lastTable.NodeTable[i].adj != NULL)
		{
			outfile << "\t\t\t\telse\n";
			outfile << "\t\t\t\t{\n";
			outfile << "\t\t\t\t\treturn \"MYERROR\";\n";
			outfile << "\t\t\t\t}\n";
		}
		outfile << "\t\t\t\tbreak;\n";
		outfile << "\t\t\t}\n";/////////case 1:{------}
	}
	outfile << "\t\t\tdefault:\n";
	outfile << "\t\t\t\treturn \"MYERROR\";\n";
	outfile << "\t\t}\n";
	outfile << "\t\tch=yytext[++i];  //ch is the next letter to be recongnized\n";
	outfile << "\t}\n";
	outfile << "\treturn \"MYERROR\";\n";
	outfile << "\n}";
}

void main()
{

}
