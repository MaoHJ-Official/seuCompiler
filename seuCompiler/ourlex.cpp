#include"graph.h"
#include<iostream>
#include<set>
#include<string>
#include<fstream>
#include<stack>
#include<vector>
using namespace std;


////����ȫ�ֱ���////
ifstream infile;
ofstream outfile;


void remove(int i, int t);
int getSI(vector<set<int>> temp, int n);//�жϱ�����ĸ�������
int getID(int label, vector< set<int> > vecset, int n);
set<int> closed(set<int> temp);
char getEdge(set<char> edge, int n);
bool equal(vector< set<int> > vecset, set<int> temp);
struct node
{
	int label;//���״̬��
	bool Isterminal;//����Ƿ�Ϊ�ս�״̬
};
//����״̬��s,��ĸ����һ��set���ϱ�ʾ,����״̬��f,ת�����������ȷ����̽��
//���ɵı߼��ǹ̶������
set<char> s;//�߼���
Graph dfaTable;
Graph nfaTable;
Graph lastTable;
//nfa��dfa����������ͨ���ߵõ��Ľ�������ǲ�һ���ģ�nfa�ǲ����ģ�dfa��Ψһ�ģ���֮�ڵ����ͨ���ߵõ�������ص�,�����ڽӱ���Խ��ͼ�ı�ʾ����






//NFAת����DFA
void NFAtoDFA()
{
	//��nfaTableͼ�ж���㣬ÿ�ι����µ�dfa���
	vector<set<int>> nfa1(100);
	nfa1[0].insert(0);//s0����I0
	set<int> current = nfa1[0];
	nfa1[0] = closed(current);
	current.clear();//��s0�ıհ�
	dfaTable.NodeTable[0].data = 0;
	dfaTable.NodeTable[0].adj = NULL;
	int NumberOfI = 0;//ȷ�����󼯺�I������

	//��I�г�������ֹ����temp��ǰ���һ�����������µļ��ϳ���
	bool flag = true;
	bool symbol = false;
	int i = 0;
	while (flag)
	{
		//��ʼ������num��ͷ�������ҳ��ڲ�ͬ���µļ��ϡ������ڼ���s�У�
		for (int edge = 0; edge < s.size(); edge++)//���������߼�
		{
			//��״̬��I�������γɵ��¸�״̬��
			for (int j = 0; j < nfa1[i].size(); j++)//I״̬�����еĽ��
			{
				int num = getID(j, nfa1, i);
				Vertex * node;
				Edge *p;
				node = &nfaTable.NodeTable[num];
				p = node->adj;
				set<int> temp;//��ʱ���ĳ�����µļ��ϣ��ж��Ƿ���ǰ��ļ�����һ��yu nfa1[i].�е�Ԫ������Ƿ���ͬ����ͬ�򴴽��µļ��ϣ�ͬ�򲻴���
				while (p != NULL)//�����������߱�
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
				//���ɵļ�����elpsion�հ�
				//�жϸü���ֵtemp�Ƿ���ǰ������еļ�����ͬ
				if (equal(nfa1, temp) && temp.size() > 0)
				{
					//��ԭ�н���²��������
					char tempedge = getEdge(s, edge);
					dfaTable.insertEdge(i, i, tempedge);
				}
				else if (temp.size() > 0)//temp��С�������0������Ϊ�ռ�����������߱���
				{
					//dfa�м���������һ,nfa1��Ҳ������һ���µı���
					NumberOfI++;
					nfa1[NumberOfI].insert(temp.begin(), temp.end());
					dfaTable.insertVertex(NumberOfI);
					if (symbol)  dfaTable.NodeTable[NumberOfI].Isterminal = 1;
					dfaTable.insertEdge(i, NumberOfI, getEdge(s, edge));
				}
				temp.clear();
			}
		}
		i++;//i��ʾ������ִ�еĽ�㣬numberofI��ʾ�����ɵĽ�㣨һ��i��ѭ���п���������� s.size()���µļ��ϣ�
		if (nfa1[i].size() <= 0) flag = false;
	}
}

set<int> closed(set<int> temp)
{
	//��ջ����õ��Ľ��
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
	return error;  // û���ҵ�
}


char getEdge(set<char> edge, int n)//��ñߵļ�������Ӧλ�õı�
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

bool equal(vector< set<int> > vecset, set<int> temp)//�ж��µļ����Ƿ���ǰ��ļ����ظ�
{

	bool flag = false;
	for (int i = 0; i < vecset.size(); i++)
	{
		if (vecset[i] == temp)  return true;
	}
	return false;
}
vector<set<int>> minidfa(100);
//��С��DFA;
void miniDFA()
{
	//�ȸ�����̬�ͷ���̬�����������������Ӽ�
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
	//��������������ѭ���Ƚ�ֱ����󼯺϶����䣬����ֻʣһ�����ȷּ������ɱ�
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
	stack<int> protect;//���漯��I��״̬�����ɼ��ϵĽڵ�����Ӧ��״̬
	stack<int> news;
	int symbolsize = minidfa.size();
	while (flag)
	{
		int flagtemp = minidfa.size();
		for (int edge = 0; edge < s.size(); edge++)//���������߼�
		{
			char tempchar = getEdge(s, edge);
			while (symbol)
			{
				//�����Ľ�㱣�棺minidfa���ӽ�㼴��
				//������̬�Ľ�㣺
				//����ж�������ѭ��minidfa
				//ѭ����Ҫ�жϵļ��ϣ��Լ�����ÿ��״̬ͨ�������ɵĺ��״̬�����������ڼ��ϣ��´����ж���һ��״̬���ɵļ��ϣ�������ͬ���������
				//����ѹ��ջ�У��鿴ջ�Ĵ�С���ڽ��бȽϣ���ջ�Ĵ�С����һ����ԭ���ļ��Ϸ��ѣ�����ֻ��֪�������ѽ����ļ��ϼ��ɡ�
				//����ջ��һ����Ŷ�Ӧ�����¸������ı�ţ���һ�������Щ����ڸñ��¶�Ӧ�ļ��ϱ��

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
								//�ж�p->dest���ĸ�����
								int SI = getSI(minidfa, p->dest);
								nextS.push(SI);
								iskongji = false;
							}
							p = p->link;
						}
						if (iskongji) nextS.push(i);
					}
					//��������ջ������һ������ջ��ջ��Ԫ�ز���i,���½�һ�����ϣ���itselfջ����Ԫ�ؼ��룬˳����ɾ��ԭ�����е�Ԫ��
					stack<int> tempstack;
					stack<int> tempnew;
					while (!nextS.empty())
					{
						int t = nextS.top();
						nextS.pop();
						int t1 = itself.top();
						itself.pop();
						if (t != i)//���t����i,����Ҫ�ж�t�Ƿ��ǰ�������״̬ת��ͬһ�����ϣ�����������Ҫ����һ���µĽ�㣬���������ԭ�е��Ǹ����
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
	//�Է��ż��е�ÿһ���߽����ж�
	//����������С����dfa��
	//��Ӷ���
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
	//��ӱ�
	for (int i = lastTable.numVertices; i > 0; i--)
	{
		//	int j=tempnode.top();
		//	tempnode.pop();
		int j = lastTable.NodeTable[i].data;
		lastTable.NodeTable[i].adj = NULL;
		//	lastTable.NodeTable[i].data=i;

		for (int z = 0; z < minidfa[j].size(); z++)//�Լ���������״̬�����ж�
		{
			int num = getID(z, minidfa, j);
			//������ÿ��״̬�ĳ��߱��е���һ��״̬���ĸ������У��ͽ����������϶�Ӧ�Ľڵ�ͨ���ñ���ϵ����
			Edge *p;
			p = dfaTable.NodeTable[num].adj;
			while (p != NULL)
			{
				int id = getSI(minidfa, p->dest);
				int nex;
				//����id,��id��Ӧ�Ľ���ҳ���
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

int getSI(vector<set<int>> temp, int n)//�жϱ�����ĸ�������
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
	//�����Եı�дҪ���Ǹ�ʶ��ı��ʽ֮��Ҫ�ӿհ׷�

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
	//outfile<<"\tint N=n+1 //N��ʾ������1��Ϊ��״̬������һ��\n";
	outfile << "\twhile(i<=yytext.length())\n";
	outfile << "\t{\n";//////////////////{
	outfile << "\t\tswitch(state)\n";
	outfile << "\t\t{\n";//////////////////{
	for (int i = 0; i < lastTable.numVertices; i++)   //��ÿһ��dfa״̬����һ��case
	{
		int flagofelseif = 0;
		outfile << "\t\tcase " << i << ":\n";

		outfile << "\t\t\t{\n";//////////////////////{
		if (lastTable.NodeTable[i].Isterminal == 1)
		{
			outfile << "\t\t\t\tif(i==yytext.length())\n";//�Ѿ�ʶ����ɴ�ӡ����Ӧ�ö���
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
