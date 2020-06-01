#pragma warning(disable:4786)
#include<iostream>
#include<ctype.h>
#include<stack>
#include<queue>
using namespace std;
#include"NFA.h"

NFA::NFA()
{
	start = NULL;
	end = NULL;
	nodenum = 0;
}

NFA::NFA(const NFA& nfa)
{
	start = nfa.start;
	end = nfa.end;
	nodenum = nfa.nodenum;
	nfaNodes = nfa.nfaNodes;

}

void NFA::change(string &r1, string &r2)
// 将正规表达式串r1(以'#'作为结束标记)转换为存于r2后缀表达式
{
	stack<string> opStack;//用于暂存运算符的栈
	opStack.push("#");  //将‘#’压入栈底
	int i, j;
	i = 0;        //指示r1中字符位置
	j = 0;        //指示r2中字符位置
	string ch;
	r2 = "";
	ch = r1[i];
	while (ch != "#")
	{//顺序处理中缀表达式中的每个字符
		if (ch == " ")
			ch = r1[++i];//对空格字符不做处理，顺序读取下一个字符
		else if (ch == "(")
		{
			opStack.push(ch);//左括号直接压入栈中
			ch = r1[++i];
		}
		else if (ch == ")")
		{//对右括号，使括号内运算符依次出栈，并写入r2中
			while (opStack.top() != "(")
			{
				r2.append(opStack.top());
				opStack.pop();
			}
			opStack.pop();
			ch = r1[++i];
		}
		else if (ch == "." || ch == "|" || ch == "*")
		{
			string w = opStack.top();
			while (priority(w) >= priority(ch))
			{//若栈顶算符优先级大于当前算符优先级，弹出输出到r2
				r2.append(w);
				opStack.pop();
				w = opStack.top();
			}
			opStack.push(ch);//将当前的算符压入栈中
			ch = r1[++i];
		}
		else
		{
			r2.append(ch);
			//j++;
			ch = r1[++i];
		}
	}

	ch = opStack.top();
	while (ch != "#") //将栈中的元素全部弹出
	{
		r2.append(opStack.top());
		opStack.pop();
		ch = opStack.top();
	}
	opStack.pop();
	r2.append("$");//结束r2
}

NFA* NFA::reToNFA(string & r2)
{
	stack<NFA> nfaStack; //定义暂存中间的生成的NFA栈
	int i;
	i = 0;   //用于指示扫描r2串的字符的位置
	string ch;
	ch = r2[i];//ch保存R2中扫描到的字符
	while (ch != "$")
	{//顺序处理后缀表达式的每个字符
		if (isletter(ch))
		{//对于字母表中的元素，作如下处理
			node *s1 = new node(++nodenum); //create a new node,its label is ++nodenum
			node *s2 = new node(++nodenum); //create a new node,its label is ++nodenum
			s1->acceptstatetag = 0;//s1为非接受态	
			(s1->firstEdge).symbol = strToChar(ch);
			(s1->firstEdge).nextnode = s2;//s1通过 a边连接到s2
			(s1->lastEdge).nextnode = '\0';//s1无其他发出边

			s2->acceptstatetag = 1;//s2为接受态
			(s2->firstEdge).nextnode = '\0';//s2无发出边
			(s2->lastEdge).nextnode = '\0';

			//this->nfaNodes.push_back(*s1);//将新生成的结点插入到结点集合中
			//this->nfaNodes.push_back(*s2);

			//this->start=s1;
			//this->end=s2;

			NFA *tempNFA = new NFA();
			tempNFA->start = s1;
			tempNFA->end = s2;
			tempNFA->nodenum = nodenum;

			nfaStack.push(*tempNFA);//将新生成的NFA压入栈 中

			ch = r2[++i];//顺序读取下一个字符

		}
		else if (ch == ".")
		{//对于'&',先从栈中弹出两个NFA，将其合并，然后压入栈中

		  //从栈中弹出两个NFA，分别存于R1和R2中
			NFA *R2 = new NFA();
			*R2 = nfaStack.top();
			nfaStack.pop();

			NFA *R1 = new NFA();
			*R1 = nfaStack.top();
			nfaStack.pop();

			//将R1和R2合并，合并后的NFA为R1
			R1->end->firstEdge.symbol = '@';
			(R1->end)->firstEdge.nextnode = R2->start;
			(R1->end->lastEdge).symbol = '\0';

			R1->end->acceptstatetag = 0;//R1的接受态设为非接受态


			//start=R1->start;
			//end=R2->end;
			R1->end = R2->end;

			R1->nodenum = nodenum;//保存当前结点的数目

			nfaStack.push(*R1);//将合并后的NFA压入栈中
			delete R1;//释放相应内存
			delete R2;
			//delete R3;
			ch = r2[++i];
		}
		else if (ch == "|")
		{
			NFA *R2 = new NFA();
			*R2 = nfaStack.top();
			nfaStack.pop();  //'@'

			NFA *R1 = new NFA();
			*R1 = nfaStack.top();
			nfaStack.pop();

			node *s3 = new node(++nodenum); //create a new node,its label is ++nodenum
			node *s4 = new node(++nodenum);
			s3->acceptstatetag = 0;
			s3->firstEdge.symbol = '@';
			s3->firstEdge.nextnode = R1->start;

			s3->lastEdge.symbol = '@';
			s3->lastEdge.nextnode = R2->start;

			s4->acceptstatetag = 1;

			R1->end->firstEdge.symbol = '@';
			R1->end->firstEdge.nextnode = s4;
			R1->end->lastEdge.nextnode = '\0';
			R1->end->acceptstatetag = 0;


			R2->end->firstEdge.symbol = '@';
			R2->end->firstEdge.nextnode = s4;
			R2->end->lastEdge.nextnode = '\0';
			R2->end->acceptstatetag = 0;//置R2->end为非接受态

			NFA *tempNFA = new NFA();
			tempNFA->start = s3;
			tempNFA->end = s4;
			tempNFA->nodenum = nodenum;
			nfaStack.push(*tempNFA);

			delete tempNFA;
			delete R1;
			delete R2;

			ch = r2[++i];

		}
		else if (ch == "*")
		{
			NFA *R1 = new NFA();
			*R1 = nfaStack.top();
			nfaStack.pop();

			node *s3 = new node(++nodenum); //create a new node,its label is ++nodenum
			node *s4 = new node(++nodenum);
			s3->acceptstatetag = 0;
			s3->firstEdge.symbol = '@';
			s3->firstEdge.nextnode = R1->start;

			s3->lastEdge.symbol = '@';
			s3->lastEdge.nextnode = s4;

			s4->acceptstatetag = 1;//s4设为接受态
			R1->end->firstEdge.symbol = '@';
			R1->end->firstEdge.nextnode = s4;
			R1->end->lastEdge.symbol = '@';
			R1->end->lastEdge.nextnode = R1->start;
			R1->end->acceptstatetag = 0;
			R1->start = s3;
			R1->end = s4;
			R1->nodenum = nodenum;
			nfaStack.push(*R1);

			delete  R1;

			ch = r2[++i];

		}
		else
			error();
	}

	NFA *nfa = new NFA();
	*nfa = nfaStack.top();
	//this->end=nfa->end;
	//this->start=nfa->start;
	nfaStack.pop();
	return nfa;
}
void NFA::join(NFA & nfa)
{
	if (this->start == NULL)
		this->start = nfa.start;
	else
	{
		node *s3 = new node(++nodenum);
		s3->acceptstatetag = 0;
		s3->firstEdge.symbol = '@';
		s3->firstEdge.nextnode = start;//第一条边指向第一个NFA的开始处
		s3->lastEdge.symbol = '@';
		s3->lastEdge.nextnode = nfa.start;//第二条边指向第二个NFA的开始处
		start = s3;

	}

}
bool NFA::isletter(string & c)
{
	const char*ptr = NULL;
	ptr = c.data();
	if (isletter(ptr[0]))
		return true;
	else
		return false;
}
bool NFA::isletter(char c)
{
	if (isalnum(c))
		return true;
	switch (c)
	{
	case '+':
		return true;
		break;
	case '-':
		return true;
		break;
	case '/':
		return true;
		break;
	case '=':
		return true;
		break;
	default:
		return false;
	}
}
char NFA::strToChar(string & str)
{
	const char*ptr = NULL;
	ptr = str.data();
	return ptr[0];
}
int NFA::priority(string c)
{
	char ch = strToChar(c);
	switch (ch)
	{
	case '#':
		return 0;
	case '|':
		return 1;
	case '.':
		return 2;
	case '*':
		return 3;
	default:
		error();
		return -1;
	}
}
void NFA::asign(NFA &nfa)
{
	nfa.end = end;
	nfa.nfaNodes = nfaNodes;
	nfa.nodenum = nodenum;
	nfa.start = start;
}
void NFA::error()
{
}

NFA& NFA::operator =(const NFA& right)
{
	if (&right != this)
	{
		end = right.end;
		nfaNodes = right.nfaNodes;
		nodenum = right.nodenum;
		start = right.start;

	}
	return *this;
}

void NFA::preProcess(string & re)//对正规表达式预处理
{
	re.append("#");
	int size = re.length();
	char *ptr = new char[2 * size];
	int j = 0;//用于指示暂存队列的游标
//	vector<char> temp;//暂存插入后的结果
	char cur = 0;//存当前取得的字符
	char next = 0;//当前字符的下一个字符

	for (int i = 0; i < size - 1; i++)
	{
		cur = re[i];
		next = re[i + 1];
		ptr[j++] = cur;
		if (isletter(cur) && isletter(next))//若当前和下一个字符都是字母、数字或运算符，怎插入'.'
			//temp.push_back('.');
			ptr[j++] = '.';
		if (cur == ')' && next == '(')
			ptr[j++] = '.';
	}
	//temp.push_back(next);
	//re.empty();
	ptr[j] = next;
	re = "";//清空当前正规表达式
	int index = 0;
	cur = ptr[index];
	string temp;
	while (cur != '#')//将处理后的正规表达式赋给re
	{
		temp = cur;
		re += temp;
		cur = ptr[++index];
	}

	re.append("#");

}


void NFA::convrtAll(map<string, string> &reTable, map<string, string> &actionTable)
{
	map<string, string>::iterator iter;
	string infix;//存中缀表达式
	string suffix;//存后缀表达式
	TerminalNode endNode;//暂存终止结点
	NFA *tempNFA = NULL;//暂存从正规表达式转换而来的NFA
	this->start = NULL;
	this->end = NULL;
	string str;
	for (iter = actionTable.begin(); iter != actionTable.end(); iter++)
	{
		str = iter->first;
		cout << iter->first << " " << iter->second << endl;
		if (isIn(reTable, str))//如果actionTable当前元素在reTable中，从reTable中取出相应的正规表达式
			infix = reTable[str];
		else
			infix = str;         //否则，取出当前actionTable的第一项

		this->preProcess(infix);//在正规表达式中插入连接运算符‘.’

		this->change(infix, suffix);//将中缀表达式转为后缀表达式 

		tempNFA = reToNFA(suffix);//将后缀表达式转换为NFA存入tempNFA中

		endNode.id = (*tempNFA).end->label;//保存终止结点编号
		endNode.type = iter->first;//保存终止结点类型

		terminalNode.push_back(endNode);

		this->join(*tempNFA);

	}

	getCharSet(reTable, actionTable);//获得输入字符集
}

bool NFA::isIn(map<string, string> reTable, string str)
{
	map<string, string>::iterator iter;
	for (iter = reTable.begin(); iter != reTable.end(); iter++)
	{
		if (iter->first == str)
			return true;
	}
	return false;
}

void NFA::printTer()
{
	vector<TerminalNode>::iterator iter;
	for (iter = terminalNode.begin(); iter != terminalNode.end(); iter++)
		cout << (*iter).id << '\t' << (*iter).type << endl;

}
/*
void NFA::preOrder(node* tree)
{
	node *left=NULL;
	node *right=NULL;
	if(tree!=NULL)
	{
		nfaNodes.push_back(*tree);
		left=(tree->firstEdge).nextnode;
		right=(tree->lastEdge).nextnode;
		if(left!=NULL)
			preOrder(left);
		if(right!=NULL)
			preOrder(right);
	}
	return ;
}*/

void NFA::preOrder(node *tree, DFA& dfa)//采用广度优先算法遍历NFA的结点，将其存储到DFA结点表中
{
	int n = this->nodenum + 1;//取结点个数
	int w = 0;
	node * neighbor = NULL;
	bool *visited = new bool[n];
	for (int i = 0; i < n; i++)
		visited[i] = false;
	int loc = tree->label;
	this->nfaNodes.push_back(tree);
	cout << "label : " << loc << endl;
	visited[loc] = true;
	stack<node*> que;
	que.push(tree);
	while (!que.empty())
	{
		node *top = que.top();
		que.pop();
		neighbor = (top->firstEdge).nextnode;

		if (neighbor != NULL)//判断第一条边是否为空
		{
			w = neighbor->label;
			if (visited[w] == false)//若未访问过
			{
				this->nfaNodes.push_back(neighbor);//压结点地址入栈
				loc = neighbor->label;
				//cout<<"label : "<<loc<<endl;//访问该节点
				que.push(neighbor);
				visited[w] = true;
			}//第一条边访问结束
			neighbor = (top->lastEdge).nextnode;
			if (neighbor != NULL)//若第二条边不为空
			{
				w = neighbor->label;
				if (visited[w] == false)//若未访问过
				{
					this->nfaNodes.push_back(neighbor);//压结点地址入栈
					loc = neighbor->label;
					//cout<<"label : "<<loc<<endl;//访问该节点
					que.push(neighbor);
					visited[w] = true;
				}
			}//第二条边访问结束
		}
	}

	delete[]visited;
}
void NFA::nfaTodfa(DFA & dfa)//将NFA的结点集赋给DFA
{
	dfa.start = start->label;
	node* ptr = start;
	nfaNodes.clear();
	preOrder(ptr, dfa);
	cout << endl << endl;
	dfaNode *tempNode = new dfaNode();

	cout << "NFA 的结点未排序前：" << endl;

	//下面是将nfa的节点表存储至dfa的结点表
	for (vector<node*>::iterator it = nfaNodes.begin(); it != nfaNodes.end(); it++)
	{
		//cout<<"label"<<'\t'<<(*it)->label<<endl;
		tempNode->acceptstatetag = (*it)->acceptstatetag;
		tempNode->label = (*it)->label;

		//cout<<"LABEL----"<<tempNode->label
		//	<<"\tTER------"<<tempNode->acceptstatetag<<endl;


		//存储结点发出边
		if (((*it)->firstEdge).nextnode != NULL)
		{
			dfaEdge edge;
			edge.symbol = ((*it)->firstEdge).symbol;
			edge.nextNode = ((*it)->firstEdge).nextnode->label;

			//cout<<"第一结点："<<edge.nextNode<<'\t';

			(tempNode->edges).push_back(edge);
			if (((*it)->lastEdge).nextnode != NULL)
			{
				edge.symbol = ((*it)->lastEdge).symbol;
				edge.nextNode = ((*it)->lastEdge).nextnode->label;

				//cout<<"第二结点："<<edge.nextNode<<'\n';

				(tempNode->edges).push_back(edge);
			}
		}//存储结点边部分结束

		//cout<<endl;

		dfa.dfaNodeTable.push_back(*tempNode);
		tempNode->edges.clear();
	}



	dfa.charSet = charSet;//将NFA字母表赋给DFA

	dfa.sortNodes();//将结点按id排序，下号标即为结点id号
	dfa.acceptedNode = this->terminalNode;
	vector<TerminalNode>::iterator iter2;
	for (iter2 = terminalNode.begin(); iter2 != terminalNode.end(); iter2++)
	{
		dfa.terminal[iter2->id] = iter2->type;
	}
	//dfa.printNode();
	//dfa.epslonClosure(14);


	delete tempNode;
}

void NFA::getCharSet(map<string, string>reTable, map<string, string>reaction)
{
	map<string, string>::iterator iter;
	string temp;//暂存字符串
	for (iter = reTable.begin(); iter != reTable.end(); iter++)
	{
		//cout<<iter->first<<" "<<iter->second<<endl;
		temp = iter->second;
		char ch = 0;
		for (int i = 0; i < temp.length(); i++)
		{
			ch = temp[i];
			if (isletter(ch))
				charSet.insert(ch);
		}
	}
	for (iter = reaction.begin(); iter != reaction.end(); iter++)
	{
		//cout<<iter->first<<" "<<iter->second<<endl;
		temp = iter->first;
		char ch = 0;
		for (int i = 0; i < temp.length(); i++)
		{
			ch = temp[i];
			if (isletter(ch))
				charSet.insert(ch);
		}
	}

	//std::ostream_iterator<char> output(cout," ");

	//cout<<"charSet contain "<<endl;
	//std::copy(charSet.begin(),charSet.end(),output);


}