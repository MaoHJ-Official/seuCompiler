/*
����cminus.y�ļ������н�����
��������ļ������������޹��ķ�����lr��1����Ŀ���Ĺ���
��lr��1����Ŀ�����ɷ�����
���ݷ��������yacc�ļ�
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

/*********���ݽṹ��������********/
fstream input;//����myyacc�ļ�

//�ռ������
set<string> terminal;
//���ռ������
set<string> Noterminal;


struct Lr1Item {//lr1��Ŀ����
	string left;//��Ŀ��
	string right[10];//��Ŀ�Ҳ�
	int rightnum;//����ʽ�Ҳ����Ÿ���
	int dotpos;//����ʽ�е�ĸ���
	set<string> predict;//��ĿԤ���
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

set<Lr1Item> Item;//ȫ�ֱ������������ʼ�Ĳ���ʽ

struct Lr1PDAState {//lr1�����Զ���״̬����
	int name;//״̬��־������������ʾ
	set<Lr1Item> items;//״̬ӵ�е���Ŀ��
	int itemsnum;//״̬ʵ��ӵ�е���Ŀ��
	bool operator < (const Lr1PDAState& pdastate)const
	{
		return items < pdastate.items;
	}
};

struct Lr1PDAedge {//lr1�����Զ����ı߶���
	int startname;//�ߵ�����־״̬
	string tag;//���ϱ��
	int endname;//�ߵ���ֹ״̬���
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



struct Lr1PDA {//lr1�����Զ���
	set<Lr1PDAState> states;//�Զ�����ӵ�е�״̬��
	int NumOfStates;
	set<Lr1PDAedge>  edges;
	int NumOfEdges;
};
Lr1PDA  pda;//�����������Զ���


struct op {//�洢������
	string dest;//��������������Ҳ�����
	int level;//levelԽ�󣬲��������ȼ�Խ��
	string name;//����������
}oper[10];


/*********����ʹ��������********/
void inputyacc();//����cminus.y�ļ�
void createLr1PDA();//����Lr1�����Զ���
void createTable();//�����﷨������
bool IsIn(string a1, set<string> a2);//�ж��ַ���a1�Ƿ����ַ�������a2��
//void first(string s);//���ظ��ַ�������λ�ռ��ַ�
void generateLr1Table();//����lr1������


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
	int line = 0;//��ʾ����������

	//��һ���ֽ�%{��}%֮��ķ��뵽��������,getlineÿ�λ�ȡ�ĵ��е�һ�д���	
	while (!(getchar[0] == '%' && getchar[1] == '}'))
	{
		line++;
		input.getline(getchar, BUFSIZ);
	}

	//�����岿�ֱ�������
	int opleftlevel = 0;
	int opnum = 0;
	line++;
	input.getline(getchar, BUFSIZ);
	while (!(getchar[0] == '%' && getchar[1] == '%'))
	{
		if (getchar[0] == '%')
		{
			char* star = strtok_s(NULL, "%", &getchar);
			char* tok = strtok_s(NULL, "\t ", &star);// ��һ��Ϊ�Ʊ��
			if (strcmp("token", tok) == 0)// token���к�ķ���Ϊ�ս��
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

	//������ʽ���ִ���
	line++;
	input.getline(getchar, BUFSIZ);
	while (!(getchar[0] == '%' && getchar[1] == '%'))
	{
		char* seg = strtok_s(NULL, "\t ", &getchar);
		//�����Ƕ�ȡ�ò���ʽ���󲿣������ð�ţ�Ȼ��һ���Ҳ������ţ�Ȼ����һ���Ҳ�����ʵ����һ������ʽ������Ƿֺţ���ʾ����
		//ͬʱ����ʽ�г��ֵķ��ռ�������Ҫ���뵽
		while (seg == NULL)
		{
			line++;
			input.getline(getchar, BUFSIZ);
			seg = strtok_s(NULL, "\t ", &getchar);
		}
		//�Ӳ�Ϊ���еĿ�ʼ
		string left;
		Lr1Item temp;//��ʱ����ʽ

		left = string(seg);
		temp.left = left;
		Noterminal.insert(left);//����set���ϵ����ʣ����������ظ��ĳ��֣�������ȥ�жϸ÷����Ƿ��Ѿ�����
		seg = strtok_s(NULL, "\t ", &getchar);//ȥ��������
		seg = strtok_s(NULL, "\t ", &getchar);
		int ptr = 0;
		while (1)
		{
			//	cout << "can I in" << endl;
			//�˼����֣���|�ȷ��ţ���Ҫ����ʶ�����  ���еľ��Ǽ��뵽�ռ����ͷ��ռ�����   
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
	//�鿴�Ƿ��ļ���β�����ǽ���������뻺����
	//���õĶ��������ٶ���
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
	//��������һ��LR(1)��Ŀ��(s'->programe,#)��״̬��Ϊ��ʼ״̬

	queue<Lr1PDAState> StateQueue;//����һ��״̬���У���ʼΪ��
	StateQueue.push(InitState);//����ʼ״̬���



	set<Lr1PDAState> ExtendedStateSet;//������������������
	while (!StateQueue.empty())
	{
		s1 = StateQueue.front();
		StateQueue.pop();
		ExtendedStateSet.insert(s1);//��������������������������

		queue<Lr1Item> ItemQueue;//����һ����Ŀ���У���ʼΪ��,����s1״̬�ڲ���Ŀ��չ
		//����״̬�е�ÿ��item�ĵ����ķ��ս��������չ
		set<Lr1Item> ::iterator it;
		for (it = s1.items.begin(); it != s1.items.end(); it++)
		{
			if (IsIn((*it).right[(*it).dotpos], Noterminal))//�жϸ÷����Ƿ��ڷ��ս��������
			{
				ItemQueue.push((*it));//s1״̬�е�ǰLR(1)��Ŀ�Ҳ���֮���Ƿ��ս������Ҫչ��
			}
		}


		set<Lr1Item> ExtendedItemSet;//��ʾ��չ����Ŀ����⣬��û�������ItemQueue���н�����չ
		while (!ItemQueue.empty())
		{
			s2 = ItemQueue.front();
			ItemQueue.pop();
			string s = s2.right[s2.dotpos];
			ExtendedItemSet.insert(s2);
			int dot = s2.dotpos;
			if (dot + 1 <= s2.rightnum)
			{
				//���ռ���������չ
				//���ռ���
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
		//��һ��״̬�ڵ����п���չ��Ŀ��չ
		pda.states.insert(s1);
		if (pda.NumOfStates == 0)
			S0 = s1;
		pda.NumOfStates++;

		//��״̬��չ
		//���֮��ķ��ϣ�ÿ������һ����
		set<string> symboledge;
		set<Lr1Item>::iterator symedge;
		for (symedge = s1.items.begin(); symedge != s1.items.end(); symedge++)
		{
			if ((*symedge).right[(*symedge).dotpos] != "")
				symboledge.insert((*symedge).right[(*symedge).dotpos]);
		}

		//��s1�ĺ��״̬�е�ÿһ���߶�Ӧ��״̬�ӽ�ȥ
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
			if (after == before + 1)//��������״̬
			{
				tempstate.name = Label;
				tempit.endname = tempstate.name;
				Label++;
				pda.edges.insert(tempit);
				StateQueue.push(tempstate);
				cout << tempstate.name << endl;
			}
			else//δ������״̬
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
	//״̬��״̬֮��Ĺ�ϵ����pda��
	set<Lr1PDAedge>::iterator ed;
	for (ed = pda.edges.begin(); ed != pda.edges.end(); ed++)
	{
		Goto[(*ed).startname].tg.push_back((*ed).tag);
		Goto[(*ed).startname].next.push_back((*ed).endname);
	}
	//�����nextΪ���Ļ���Ϊ�ƽ���nextΪ���Ļ�����ʾ��Լ
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
					if ((*it).dotpos == (*it).rightnum)//��Ҫ��Լ����Ŀ
					{
						//���������Ŀ��״̬0��Ϊ�ڼ������ʽ
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
						for (r = (*it).predict.begin(); r != (*it).predict.end(); r++)//�����ƽ���Լ��ͻ�͹�Լ��Լ��ͻ
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
							if (sy)//���ڳ�ͻ���ж����ȼ����ٽ����ƽ����ǹ�Լ
							{
								Goto[i].next[tt] = gg;

							}
							else//�����ڳ�ͻ��ֱ�ӹ�Լ
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
