/*
读入cminus.y文件，进行解析；
解析后的文件利用上下文无关文法进行lr（1）项目集的构造
从lr（1）项目集生成分析表
根据分析表输出yacc文件
*/

#include<iostream>
#include<fstream>
#include<set>
#include<string>
#include<queue>
#include<vector>
#include<stack>
#include<map>
using namespace std;

/*********数据结构体声明段********/
fstream input;//读入myyacc文件

//终极符存放
set<string> terminal;
//非终极符存放
set<string> Noterminal;


struct Lr1Item {//lr1项目定义
	string left;//项目左部
	string right[10];//项目右部
	int rightnum;//产生式右部符号个数
	int dotpos;//产生式中点的个数
	set<string> predict;//项目预测符
	bool operator<(const Lr1Item& item) const
	{
		if (strcmp(left.c_str(), item.left.c_str()) == 0)
		{


			int i = 0;
			while (i < 10)
			{
				if (strcmp(item.right[i].c_str(), right[i].c_str()) != 0)
					return right[i] < item.right[i];

				i++;
			}
			if (predict == item.predict)
				return dotpos < item.dotpos;

			return predict < item.predict;

		}
		return left < item.left;
	}


};

set<Lr1Item> Item;//全局变量，保存最初始的产生式

struct Lr1PDAState {//lr1下推自动机状态定义
	int name;//状态标志名，用整数表示
	set<Lr1Item> items;//状态拥有的项目数
	int itemsnum;//状态实际拥有的项目数
	bool operator < (const Lr1PDAState& pdastate)const
	{
		return items < pdastate.items;
	}
};

struct Lr1PDAedge {//lr1下推自动机的边定义
	int startname;//边的起点标志状态
	string tag;//边上标记
	int endname;//边的终止状态标记
	bool operator<(const Lr1PDAedge& ed) const
	{
		if (startname == ed.startname)
		{
			if (endname == ed.endname)
			{
				return tag < ed.tag;
			}
			return endname < ed.endname;
		}
		return startname < ed.startname;
	}
};



struct Lr1PDA {//lr1下推自动机
	set<Lr1PDAState> states;//自动机所拥有的状态数
	int NumOfStates;
	set<Lr1PDAedge>  edges;
	int NumOfEdges;
};
Lr1PDA  pda;//最后的完整的自动机


struct op {//存储操作符
	string dest;//左操作符，还是右操作符
	int level;//level越大，操作符优先级越高
	string name;//操作符名称
}oper[10];


/*********函数使用声明段********/
void inputyacc();//解析cminus.y文件
void createLr1PDA();//构造Lr1下推自动机
void createTable();//生成语法分析表
bool IsIn(string a1, set<string> a2);//判断字符串a1是否在字符串集合a2中
//void first(string s);//返回该字符串的首位终极字符
void generateLr1Table();//构造lr1分析表


bool IsIn(string a1, set<string> a2)
{
	set<string>::iterator current;
	for (current = a2.begin(); current != a2.end(); current++)
	{
		if (*current == a1)
			return true;
	}
	return false;
}

void first(vector<string> a, set<string> orignal, set<string>& nextpredict)
{

	if (IsIn(a[0], terminal))
	{
		nextpredict.insert(a[0]);
		return;
	}
	if (a[0] == "")
	{
		set<string>::iterator it;
		for (it = orignal.begin(); it != orignal.end(); it++)
		{
			nextpredict.insert((*it));
		}
		return;
	}
	stack<string> noTer;
	noTer.push(a[0]);
	set<string> visited;
	while (!noTer.empty())
	{
		string left = noTer.top();
		noTer.pop();
		visited.insert(left);
		set<Lr1Item>::iterator it;
		for (it = Item.begin(); it != Item.end(); it++)
		{
			if ((*it).left == left)
			{
				string righttemp = (*it).right[0];
				if (IsIn(righttemp, Noterminal))
				{
					nextpredict.insert(righttemp);
				}
				else if (visited.count(righttemp) == 0)
				{
					noTer.push(righttemp);
				}
			}
		}
	}
	/*
	else
	{
		set<Lr1Item>::iterator it;
		for(it=Item.begin();it!=Item.end();it++)
		{
			if(a[0]==(*it).left)
			{
				if((*it).right[0]=="eplsion")
				{
					a.pop_back();
					cout<<1<<endl;
					first(a,orignal,nextpredict);

				}
				else
				{
					if(IsIn((*it).right[0],Noterminal))
					{

						if((*it).right[0]!=a[0])
						{
							vector<string> temp;
							temp.push_back((*it).right[0]);
							first(temp,orignal,nextpredict);
						}
					}
					if(IsIn((*it).right[0],terminal))
					{
						nextpredict.insert((*it).right[0]);
					}
				}
			}
		}
	}*/
}

void inputyacc()
{
	string* buff = new string[BUFSIZ];
	//input.open("cYacc.y", ios::in);
	input.open("minic.y", ios::in);
	char* getchar = new char[BUFSIZ];
	input.getline(getchar, BUFSIZ);
	int line = 0;//表示读到的行数

	//第一部分将%{和}%之间的放入到缓冲区中,getline每次获取文档中的一行代码	
	while (!(getchar[0] == '%' && getchar[1] == '}'))
	{
		line++;
		input.getline(getchar, BUFSIZ);
	}

	//将定义部分保存起来
	int opleftlevel = 0;
	int opnum = 0;
	line++;
	input.getline(getchar, BUFSIZ);
	while (!(getchar[0] == '%' && getchar[1] == '%'))
	{
		if (getchar[0] == '%')
		{
			char* star = strtok_s(NULL, "%", &getchar);
			char* tok = strtok_s(NULL, "\t ", &star);// 第一个为制表符
			if (strcmp("token", tok) == 0)// token序列后的符合为终结符
			{
				tok = strtok_s(NULL, "\t ", &star);
				while (tok != NULL)
				{
					terminal.insert(tok);
					tok = strtok_s(NULL, "\t ", &star);
				}
			}
			else if (strcmp("left", tok) == 0)
			{
				tok = strtok_s(NULL, "\t ", &star);
				while (tok != NULL && tok[0] != '/')
				{
					oper[opnum].name.operator=(tok);
					oper[opnum].level = opleftlevel;
					oper[opnum].dest.operator=("left");
					opnum++;
					tok = strtok_s(NULL, "\t ", &star);
				}
				opleftlevel++;
			}
			else  if (strcmp("nonassoc", tok) == 0)
			{

				tok = strtok_s(NULL, "\t ", &star);
				while (tok != NULL && tok[0] != '/')
				{
					tok = strtok_s(NULL, "\t ", &star);
				}
				opleftlevel++;
			}
			else  if (strcmp("union", tok) == 0)
			{

			}
			else if (strcmp("type", tok) == 0)
			{

			}
		}
		line++;
		input.getline(getchar, BUFSIZ);
	}

	//将产生式部分处理
	line++;
	input.getline(getchar, BUFSIZ);
	while (!(getchar[0] == '%' && getchar[1] == '%'))
	{
		char* seg = strtok_s(NULL, "\t ", &getchar);
		//首先是读取该产生式的左部，其次是冒号，然后一个右部，竖号，然后另一个右部，其实是另一个产生式，最后是分号，表示结束
		//同时产生式中出现的非终极符，需要存入到
		while (seg == NULL)
		{
			line++;
			input.getline(getchar, BUFSIZ);
			seg = strtok_s(NULL, "\t ", &getchar);
		}
		//从不为空行的开始
		string left;
		Lr1Item temp;//临时产生式

		left = string(seg);
		temp.left = left;
		Noterminal.insert(left);//利用set集合的性质，不允许有重复的出现，避免了去判断该符号是否已经存在
		seg = strtok_s(NULL, "\t ", &getchar);//去除“：”
		seg = strtok_s(NULL, "\t ", &getchar);
		int ptr = 0;
		while (1)
		{
			//	cout << "can I in" << endl;
			//此间会出现：，|等符号，需要将其识别出来  还有的就是加入到终极符和非终极符中   
			if (strcmp(seg, "|") == 0 || strcmp(seg, ";") == 0)
			{
				if (ptr > 0)
				{
					cout << temp.left << "->";
					int j = 0;
					while (j < 10)
					{
						if (strlen(temp.right[j].c_str()) == 0)
							break;
						cout << temp.right[j] << " ";
						j++;
					}
					temp.dotpos = 0;
					temp.rightnum = j;
					cout << temp.rightnum << endl;
					Item.insert(temp);
					int i = 0;
					while (i < 10)
					{

						cout << temp.right[i] << " ";
						temp.right[i] = "";
						i++;
					}
					cout << endl;
				}

				ptr = 0;
				if (strcmp(seg, ";") == 0)
					break;
				seg = strtok_s(NULL, "\t ", &getchar);
				if (seg == NULL)
					seg = (char*)"eplison";
			}
			temp.right[ptr].operator=(seg);
			if (string(seg).length() > 1)
			{
				if (!IsIn(string(seg), terminal))
				{
					if (!IsIn(string(seg), Noterminal))
						Noterminal.insert(string(seg));
				}
			}
			ptr++;
			seg = strtok_s(NULL, "\t ", &getchar);
			while (seg == NULL)
			{
				line++;
				input.getline(getchar, BUFSIZ);
				seg = strtok_s(NULL, "\t ", &getchar);
			}
		}
		line++;
		input.getline(getchar, BUFSIZ);
	}
	//查看是否到文件结尾，若是结束否则读入缓冲区
	//米用的东西，不再读入
}

Lr1PDAState S0;
void createPDA()
{
	Lr1PDAState InitState, s1;
	Lr1Item s2;
	int Label = 0;
	pda.NumOfStates = 0;
	pda.NumOfEdges = 0;

	Lr1Item item;
	item.left.operator=("S'");
	item.right[0].operator=("program");
	//	item.right[0].operator=("S");
	item.rightnum = 1;
	item.dotpos = 0;
	item.predict.insert("#");
	InitState.items.insert(item);
	InitState.itemsnum = 1;
	InitState.name = Label;
	Label++;
	//构建包含一个LR(1)项目：(s'->programe,#)的状态作为初始状态

	queue<Lr1PDAState> StateQueue;//建立一个状态队列，初始为空
	StateQueue.push(InitState);//将初始状态入队



	set<Lr1PDAState> ExtendedStateSet;//？？？？？？？？？
	while (!StateQueue.empty())
	{
		s1 = StateQueue.front();
		StateQueue.pop();
		ExtendedStateSet.insert(s1);//？？？？？？？？？？？？？

		queue<Lr1Item> ItemQueue;//建立一个项目队列，初始为空,用于s1状态内部项目扩展
		//对于状态中的每个item的点后面的非终结符进行扩展
		set<Lr1Item> ::iterator it;
		for (it = s1.items.begin(); it != s1.items.end(); it++)
		{
			if (IsIn((*it).right[(*it).dotpos], Noterminal))//判断该符号是否在非终结符集合中
			{
				ItemQueue.push((*it));//s1状态中当前LR(1)项目右部点之后是非终结符，需要展开
			}
		}


		set<Lr1Item> ExtendedItemSet;//表示扩展的项目，检测，若没有则加入ItemQueue队列进行扩展
		while (!ItemQueue.empty())
		{
			s2 = ItemQueue.front();
			ItemQueue.pop();
			string s = s2.right[s2.dotpos];
			ExtendedItemSet.insert(s2);
			int dot = s2.dotpos;
			if (dot + 1 <= s2.rightnum)
			{
				//是终极符，不扩展
				//非终极符
				if (IsIn(s2.right[dot], Noterminal))
				{
					set<string> nextpredict;
					vector<string> a1;
					for (int m = s2.dotpos; m < s2.rightnum; m++)
					{
						a1.push_back(s2.right[m + 1]);
					}
					first(a1, s2.predict, nextpredict);
					set<Lr1Item>::iterator it;
					for (it = Item.begin(); it != Item.end(); it++)
					{
						if ((*it).left == s2.right[dot])
						{
							Lr1Item tempitem;
							tempitem.dotpos = (*it).dotpos;
							tempitem.left = (*it).left;
							tempitem.rightnum = (*it).rightnum;
							set<string>::iterator temp;
							for (temp = nextpredict.begin(); temp != nextpredict.end(); temp++)
							{
								tempitem.predict.insert((*temp));
							}
							int j = 0;
							while (j < 10)
							{
								tempitem.right[j] = (*it).right[j];
								j++;
							}
							s1.items.insert(tempitem);
							int before = ExtendedItemSet.size();
							ExtendedItemSet.insert(tempitem);
							int after = ExtendedItemSet.size();
							if (after == before + 1)
								ItemQueue.push(tempitem);

						}
					}

				}

			}
		}
		//对一个状态内的所有可扩展项目扩展
		pda.states.insert(s1);
		if (pda.NumOfStates == 0)
			S0 = s1;
		pda.NumOfStates++;

		//对状态扩展
		//求点之后的符合，每个符合一条边
		set<string> symboledge;
		set<Lr1Item>::iterator symedge;
		for (symedge = s1.items.begin(); symedge != s1.items.end(); symedge++)
		{
			if ((*symedge).right[(*symedge).dotpos] != "")
				symboledge.insert((*symedge).right[(*symedge).dotpos]);
		}

		//将s1的后继状态中的每一条边对应的状态加进去
		set<string>::iterator tp;
		for (tp = symboledge.begin(); tp != symboledge.end(); tp++)
		{
			Lr1PDAedge  tempit;
			Lr1PDAState tempstate;
			set<Lr1Item>::iterator sy;
			int num = 0;
			for (sy = s1.items.begin(); sy != s1.items.end(); sy++)
			{
				if ((*tp) == (*sy).right[(*sy).dotpos])
				{
					Lr1Item tempitem;
					tempitem.dotpos = (*sy).dotpos + 1;
					tempitem.left = (*sy).left;
					tempitem.rightnum = (*sy).rightnum;
					set<string>::iterator it;
					for (it = (*sy).predict.begin(); it != (*sy).predict.end(); it++)
						tempitem.predict.insert((*it));
					int i = 0;
					while (i < 10)
					{
						tempitem.right[i] = (*sy).right[i];
						i++;
					}

					tempstate.items.insert(tempitem);
					// tempstate.items.insert((*sy));
					num++;
				}
			}

			tempstate.itemsnum = num;

			tempit.tag = (*tp);

			tempit.startname = s1.name;
			tempstate.name = Label;
			int before = ExtendedStateSet.size();
			ExtendedStateSet.insert(tempstate);
			int after = ExtendedStateSet.size();
			cout << "after: " << after << endl;
			cout << "before: " << before << endl;
			if (after == before + 1)//产生的新状态
			{
				tempstate.name = Label;
				tempit.endname = tempstate.name;
				Label++;
				pda.edges.insert(tempit);
				StateQueue.push(tempstate);
				cout << tempstate.name << endl;
			}
			else//未产生新状态
			{
				set<Lr1PDAState>::iterator it;
				for (it = ExtendedStateSet.begin(); it != ExtendedStateSet.end(); it++)
				{
					bool symbol = true;
					if (tempstate.items.size() == (*it).items.size())
					{
						int kk = tempstate.items.size();
						Lr1Item* tempp = new Lr1Item[kk];
						set<Lr1Item>::iterator ittemp;
						int tt = 0;
						for (ittemp = tempstate.items.begin(); ittemp != tempstate.items.end(); ittemp++)
						{
							tempp[tt] = (*ittemp);
							tt++;
						}
						set<Lr1Item>::iterator ittemp2;
						int zz = 0;
						for (ittemp2 = (*it).items.begin(); ittemp2 != (*it).items.end(); ittemp2++)
						{
							if (tempp[zz] < (*ittemp2) || (*ittemp2) < tempp[zz])
								symbol = false;

							zz++;
						}
						if (symbol)
						{
							tempit.endname = (*it).name;
							pda.edges.insert(tempit);
						}

					}
				}
			}
			pda.NumOfEdges++;

		}
	}


}

void createTable()
{
	struct table {
		vector<string> tg;
		vector<int> next;
	};
	table* Goto = new table[pda.NumOfStates];
	//状态和状态之间的关系存在pda中
	set<Lr1PDAedge>::iterator ed;
	for (ed = pda.edges.begin(); ed != pda.edges.end(); ed++)
	{
		Goto[(*ed).startname].tg.push_back((*ed).tag);
		Goto[(*ed).startname].next.push_back((*ed).endname);
	}
	//再这里，next为正的话，为移近，next为负的话，表示规约
	ofstream out("table.txt");
	int s1 = terminal.size();
	int s2 = Noterminal.size();
	int s = s1 + s2 + 1;
	cout << pda.NumOfStates << "\nState" << "	";
	out << pda.NumOfStates << "\nState" << "	";
	set<string>::iterator it;
	for (it = terminal.begin(); it != terminal.end(); it++)
	{
		cout << (*it) << "	";
		out << (*it) << "	";
	}
	cout << "#" << "	";
	out << "#" << "	";
	for (it = Noterminal.begin(); it != Noterminal.end(); it++)
	{
		cout << (*it) << "	";
		out << (*it) << "	";
	}
	cout << endl;
	out << endl;
	int* action = new int[terminal.size() + 1];
	int row = pda.NumOfStates;
	for (int i = 0; i < pda.NumOfStates; i++)
	{
		out << i << "	";
		cout << i << "	";

		set<Lr1PDAState>::iterator  tempstate;
		for (tempstate = pda.states.begin(); tempstate != pda.states.end(); tempstate++)
		{
			if ((*tempstate).name == i)
			{
				set<Lr1Item>::iterator it;
				for (it = (*tempstate).items.begin(); it != (*tempstate).items.end(); it++)
				{
					if ((*it).dotpos == (*it).rightnum)//需要规约的项目
					{
						//计算出该项目在状态0中为第几个表达式
						int tem = 1;
						set<Lr1Item>::iterator dd;
						for (dd = S0.items.begin(); dd != S0.items.end(); dd++)
						{
							bool flag = true;
							if ((*it).left == (*dd).left)
							{
								int ii = 0;
								while (ii < 10)
								{
									if ((*it).right[ii] != (*dd).right[ii])
										flag = false;
									ii++;
								}
							}
							else
								flag = false;

							if (flag)
							{
								if ((*it).left == "S'")
									tem = 0;
								break;
							}
							tem++;
						}
						set<string>::iterator r;
						for (r = (*it).predict.begin(); r != (*it).predict.end(); r++)//处理移近规约冲突和规约规约冲突
						{
							bool sy = false;
							int tt;
							for (tt = 0; tt < Goto[i].next.size(); tt++)
							{
								if (Goto[i].tg[tt] == (*r))
								{
									sy = true;
									break;
								}
							}
							int gg = 0 - tem;
							string ff = (*r);
							if (sy)//存在冲突，判断优先级，再进行移近还是规约
							{
								Goto[i].next[tt] = gg;

							}
							else//不存在冲突，直接规约
							{
								Goto[i].next.push_back(gg);
								Goto[i].tg.push_back(ff);
							}

						}
					}
				}

			}

		}
		for (int c = 0; c < Goto[i].next.size(); c++)
		{
			if (Goto[i].next[c] == 0)
			{
				cout << "	" << "acc" << "	" << Goto[i].tg[c] << "	";
				out << "	" << "acc" << "	" << Goto[i].tg[c] << "	";
			}
			else if (Goto[i].next[c] > 0)
			{
				cout << "	" << Goto[i].next[c] << "	" << Goto[i].tg[c] << "	";
				out << "	" << Goto[i].next[c] << "	" << Goto[i].tg[c] << "	";
			}
			else
			{
				cout << "	" << Goto[i].next[c] << "	" << Goto[i].tg[c] << "	";
				out << "	" << Goto[i].next[c] << "	" << Goto[i].tg[c] << "	";
			}
		}

		out << endl;
		cout << endl;
	}
}

int main()
{
	inputyacc();
	createPDA();
	createTable();
	return 0;
}
