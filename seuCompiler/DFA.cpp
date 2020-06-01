#include"DFA.h"
struct stateSet//最小化时的一个状态集
{
	int id;
	int isTer;
	int targetNum;
	set<int> states;
	stateSet()
	{
		id = 0;
		isTer = 0;
		targetNum = 0;
	}
	stateSet(const stateSet & s)
	{
		id = s.id;
		isTer = s.id;
		targetNum = s.targetNum;
		states = s.states;
	}
};
DFA::DFA()
{

}

void DFA::minimizeDfa()
{
	/***************下面这段代码将DFA状态转换为以0开始的******************/
	int size = this->dfaNodeTable.size();
	int startId = dfaNodeTable[0].label;
	for (int m = 0; m < size; m++)
	{
		dfaNodeTable[m].label -= startId;//将该结点标号减去初始值
		int edgeNum = dfaNodeTable[m].edges.size();
		for (int edgeCounter = 0; edgeCounter < edgeNum; edgeCounter++)//将各个后继结点标号减去初始值
		{
			dfaNodeTable[m].edges[edgeCounter].nextNode -= startId;
			char p = dfaNodeTable[m].edges[edgeCounter].symbol;
			int  q = dfaNodeTable[m].edges[edgeCounter].nextNode;
			dfaNodeTable[m].hashedges[p] = q;
		}
	}
	map<int, string>::iterator it1;
	map<int, string> *tempTerminal = new map<int, string>;
	int tempId;
	for (it1 = this->terminal.begin(); it1 != this->terminal.end(); it1++)
	{
		tempId = it1->first;
		tempId -= startId;
		(*tempTerminal)[tempId] = it1->second;
	}
	this->terminal = *tempTerminal;
	delete tempTerminal;

	map<int, int>::iterator it2;
	map<int, int>*tempAcc = new map<int, int>;
	for (it2 = isAcc.begin(); it2 != isAcc.end(); it2++)//
	{
		tempId = it2->first;
		tempId -= startId;
		(*tempAcc)[tempId] = it2->second;
	}
	this->isAcc = *tempAcc;
	delete tempAcc;
	/******************************************************/
	//this->printNode();/////////打印各节点情况，测试用。。。。。。。





 //	map<set<int>,int> idTab;//状态集与其标号的对应表

	 //map<int,int> numTab;//状态集标号与其内部状态数的对应表

	vector<stateSet> devide;//存分划的表
	map<string, set<int>*> terType;//终止结点类型与其对应的状态集指针
	stateSet tempSet;
	vector<int> stateToSet(size, -1);//状态标号和所属状态集号的对应表，


	int setId = dfaNodeTable[size - 1].label + 1;//初始化状态集的ID


	/*****下面是将所有状态划分为非终止结点集和终止结点集，终止结点要按终止类型划分为多个****/
	map<string, set<int>*>::iterator terIt;
	for (it1 = this->terminal.begin(); it1 != this->terminal.end(); it1++)
	{
		terIt = terType.find(it1->second);
		if (terIt == terType.end())
			terType[it1->second] = new set<int>;

		terType[it1->second]->insert(it1->first);
	}

	set<int> unTer;
	//map<int,int>::iterator it2;
	for (it2 = isAcc.begin(); it2 != isAcc.end(); it2++)//找出初始的非终止状态集
	{
		if (it2->second != 1)
		{
			unTer.insert(it2->first);
			stateToSet[it2->first] = setId;
			//cout<<it2->first<<"*****\n";
		}
	}
	tempSet.id = setId++;
	tempSet.states = unTer;
	devide.push_back(tempSet);//将初始的非终止状态集插入分划表
 //   idTab[tempSet.states]=setId++;//状态集与其标号的对应表

	map<string, set<int>*>::iterator it3;
	for (it3 = terType.begin(); it3 != terType.end(); it3++)//将初始的终止状态集插入分划表
	{
		tempSet.id = setId;
		tempSet.isTer = 1;
		//unTer=
		tempSet.states = *(it3->second);/////////////////////////////？？？？？？？？？？？//////////////////////////////////此处有问题
		devide.push_back(tempSet);
		set<int>::iterator  riter;
		for (riter = tempSet.states.begin(); riter != tempSet.states.end(); riter++)
		{
			stateToSet[*riter] = setId;//将终止状态插入状态与状态集对应表
		}
		setId++;
		//		idTab[tempSet.states]=setId++;//填入状态集与其标号的对应表
	}

	int beforeNum = 0;//一次分划前的状态集数
	int afterNum = devide.size();//一次分划后的状态集数

	//vector<stateSet> newSet;//扫描一遍分划产生的新的状态集

	set<int>::iterator qit;


	for (int k = 0; k < afterNum; k++)
	{
		cout << "第" << k << "个分划   id    " << devide[k].id << '\n';

		for (qit = devide[k].states.begin(); qit != devide[k].states.end(); qit++)
			cout << *qit << "    ";
		cout << endl;

	}

	cout << "分划数为*********" << afterNum << endl;

	for (int j = 0; j < stateToSet.size(); j++)//输出接点号与结点集号
	{
		cout << "结点" << j << "  集合 " << stateToSet[j] << endl;
	}






	map<int, set<int>*> setTab;
	set<int> Set;
	set<char>::iterator alphIter;//扫描各个输入字母的遍历器

	/************下面是算法的核心部分*************/
	while (beforeNum != afterNum)//没有新的状态集产生时，循环终止
	{
		beforeNum = afterNum;
		//	newSet.clear();
		for (int i = 0; i < beforeNum; i++)//扫描一遍各个状态集
		{
			tempSet = devide[i];
			set<int>::iterator stateIter;//扫描各个状态的遍历器


			for (alphIter = charSet.begin(); alphIter != charSet.end(); alphIter++)
			{ //针对字母表中每个输入符号,对状态集中每个状态扫描一遍，判断其后继结点所在集合
				//cout<<*alphIter<<endl;
				Set.clear();
				map<int, set<int>*>::iterator deIter;
				for (deIter = setTab.begin(); deIter != setTab.end(); deIter++)//释放掉动态内存
				{
					delete deIter->second;
				}
				setTab.clear();
				int idOfSet = -1;
				for (stateIter = tempSet.states.begin(); stateIter != tempSet.states.end(); stateIter++)
				{//开始对状态集中每个状态的扫描
					int stateId = *stateIter;
					char letter = *alphIter;//暂存该字母
					int nextId = dfaNodeTable[stateId].nextId(letter);//获得后继结点的ID标号

					if (nextId != -1)//后继结点存在
						idOfSet = stateToSet[nextId];
					else
						idOfSet = -1;
					if (!isIn(Set, idOfSet))
					{
						Set.insert(idOfSet);//插入状态集号
						setTab[idOfSet] = new set<int>;
					}
					setTab[idOfSet]->insert(stateId);

				}//结束对状态集中每个状态的扫描

				int sizeOfSet = Set.size();
				if (sizeOfSet > 1)//将新生成的状态集加入分划
				{
					stateSet temp;
					temp.id = tempSet.id;
					temp.states = *(setTab.begin()->second);
					devide[i] = temp;//将刚扫描的状态集置换
					tempSet = temp;
					map<int, set<int>*>::iterator siter;
					set<int>::iterator qiter;
					for (siter = ++setTab.begin(); siter != setTab.end(); siter++)
					{   //将新生成的状态集加入分划中
						temp.id = setId++;
						temp.states = *(siter->second);
						for (qiter = temp.states.begin(); qiter != temp.states.end(); qiter++)
							stateToSet[*qiter] = temp.id;
						devide.push_back(temp);
					}
					//break;
				}//新生成的状态集加入分划结束	


			}//结束对每个输入字母的扫描

		}//结束对每个状态集的扫描

		afterNum = devide.size();
	}

	cout << "最小化第一步完成!!!\n";
	cout << "分划数为*********" << afterNum << endl;

	for (int j = 0; j < stateToSet.size(); j++)//下面根据分划表生成dfa结点表
	{
		cout << "结点" << j << "  集合 " << stateToSet[j] << endl;
	}

	for (int k = 0; k < afterNum; k++)
	{
		cout << "第" << k << "个分划\n";
		set<int>::iterator qit;
		for (qit = devide[k].states.begin(); qit != devide[k].states.end(); qit++)
			cout << *qit << "    ";
		cout << endl;

	}


	cout << "下面根据分划开始生成最小化的DFA：\n";

	for (it2 = isAcc.begin(); it2 != isAcc.end(); it2++)//
	{
		cout << "结点   " << it2->first << "      " << it2->second << endl;
	}


	cout << "下面终止类型\n";
	for (it1 = this->terminal.begin(); it1 != this->terminal.end(); it1++)
	{
		cout << "结点   " << it1->first << "      " << it1->second << endl;
	}


	vector<dfaNode> tempdfa;
	dfaNode tempNode;
	int terNumber = 0;
	string accType;
	map<int, string> tempTerType;


	cout << "下面输出ACC情况\n";
	map<int, int>::iterator mpit;
	for (mpit = this->isAcc.begin(); mpit != this->isAcc.end(); mpit++)
		cout << mpit->first << "  is  " << mpit->second << endl;


	set<string> terTemp;
	for (int index = 0; index < afterNum; index++)//扫描分划
	{
		terTemp.clear();
		for (qit = devide[index].states.begin(); qit != devide[index].states.end(); qit++)
		{//扫描每个状态集，找出开始状态集，并记录含有终止结点的终止类型
			if (*qit == 0)
				start = devide[index].id;
			if (isAcc[*qit] == 1)
			{
				devide[index].isTer = 1;
				accType = this->terminal[*qit];
				terTemp.insert(accType);
			}

			cout << *qit << " # ";
		}
		cout << endl;

		tempNode.label = devide[index].id;
		if (terTemp.size() < 1)
			tempNode.acceptstatetag = 0;
		else
		{
			tempNode.acceptstatetag = 1;
			tempTerType[tempNode.label] = accType;
		}
		tempdfa.push_back(tempNode);
	}

	this->terminal = tempTerType;


	//this->printNode();

	dfaEdge tempEdge;
	//下面生成最小化DFA的边
	for (int index = 0; index < afterNum; index++)//扫描生成的结点
	{
		qit = devide[index].states.begin();
		int stateID = *qit;//从结点集中去一个代表
		int nextID = -1;
		for (alphIter = charSet.begin(); alphIter != charSet.end(); alphIter++)
		{
			nextID = this->dfaNodeTable[stateID].nextId(*alphIter);//计算该字母对应的下一个结点标号
			if (nextID != -1)//存在后继结点
			{
				tempEdge.symbol = *alphIter;
				tempEdge.nextNode = stateToSet[nextID];
				tempdfa[index].edges.push_back(tempEdge);
			}

		}
	}
	this->dfaNodeTable = tempdfa;
	this->printNode();



	cout << "start node is" << this->start << endl;


}

void DFA::getCharSet()
{

}
void DFA::subsetCon()//采用子集构造法由NFA生成DFA
{
	map<set<int>, int > Dstates;//保存生成的DFA结点
	map<int, set<int> > states;
	deque< int > unmarkStates;//生成状态中的未标记状态

	set<int> T = epslonClosure(start);//求开始状态的epslon闭包
	int nodeId = this->dfaNodeTable.size();//为了避免新生成的结点ed与原来结点id的冲突
	set<int> tempClosure;    //暂存生成的epslon闭包
	Dstates[T] = nodeId;       //将初始状态加入Dstates
	unmarkStates.push_back(nodeId);//在未标记状态集中加入该状态
	states[nodeId] = T;

	vector<dfaNode> Dtran;//存生成的DFA表*******重要数据最小化时要用
	//dfaNode * nodePtr=new dfaNode();
	//nodePtr->label=nodeId;
	//nodePtr->acceptstatetag=0;
	dfaNode* nodePtr = NULL;
	string terType;//终止结点集类型

	map<int, string> terSet;//存终止结点集
	TerminalNode * terPtr = NULL;//指向新生成的终止结点
	int number = 0; //每个终止结点集中终止状态个数

	while (!unmarkStates.empty())//
	{
		int unmark = unmarkStates.front();//取出一个未标记的结点
		unmarkStates.pop_front();//将该结点从未标记队列中弹出

	   /*
		dfaEdge tempEdge;
		tempEdge.symbol=*iter;
		tempEdge.nextNode=nodeId;
		nodePtr->edges.push_back(tempEdge);*/

		nodePtr = new dfaNode(unmark); //建立新结点
		vector<int> tempTer;
		if (isTerminate(states[unmark], tempTer, number))//若当前未标记的状态集中含有终止结点
		{

			nodePtr->acceptstatetag = 1;//则弹出该状态集对应结点为终止态
			if (number == 1)
				terType = terminal[tempTer[0]];
			else
			{
				string s = terminal[tempTer[0]];
				if (s != "id")
					terType = s;
				else
					terType = terminal[tempTer[1]];

			}

			terSet[unmark] = terType;//在终止结点集合中加入该结点

		}


		set<char>::iterator iter;
		for (iter = charSet.begin(); iter != charSet.end(); iter++)//对字母表中的每个输入符号
		{
			T = states[unmark];//取出未标记的状态集
			tempClosure = move(T, *iter);
			if (!tempClosure.empty())//结点集中有标号当前字母的发出边
			{
				tempClosure = this->epslonClosure(move(T, *iter));//求其epslon闭包
				map<set<int>, int>::iterator it = Dstates.find(tempClosure);
				if (it == Dstates.end())//新生成的状态集不在Dstates中
				{
					nodeId++;
					Dstates[tempClosure] = nodeId;//将新生成的状态加入Dstates中
					states[nodeId] = tempClosure;
					unmarkStates.push_back(nodeId);//将新生成的状态加入未标记状态集中

					dfaEdge tempEdge;
					tempEdge.symbol = *iter;
					tempEdge.nextNode = nodeId;
					nodePtr->edges.push_back(tempEdge);//将该生成边加入结点边集中

				}
				else//新生成的状态集在Dstates中
				{
					dfaEdge tempEdge;
					tempEdge.symbol = *iter;
					tempEdge.nextNode = Dstates[tempClosure];
					nodePtr->edges.push_back(tempEdge);//将该生成边加入结点边集中
				}


			}

		}//字母表输入符号搜索完毕

		Dtran.push_back(*nodePtr);//将刚标记的结点加入DFA表中
		delete nodePtr;//释放临时生成的结点
	}

	vector<dfaNode>::iterator nodeiter;

	cout << endl << endl;
	this->isAcc.clear();
	//暂时注释掉
	//cout<<"确定化后的DFA结点情况如下\n";
	for (nodeiter = Dtran.begin(); nodeiter != Dtran.end(); nodeiter++)
	{

		isAcc[nodeiter->label] = nodeiter->acceptstatetag;//加入终止结点表

		//cout<<nodeiter->label<<"     "<<nodeiter->acceptstatetag<<"    "
		//	<<nodeiter->edges<<endl;
	}

	map<int, string>::iterator terit;
	//for(terit=acceptedNode.begin();terit!=acceptedNode.end();terit++)
	//{
	//	cout<<terit->type<<"   "<<terit->id<<endl;
	//}

	/*
	for(terit=terSet.begin();terit!=terSet.end();terit++)
	{
		cout<<terit->first<<"   "<<terit->second<<endl;
	}*/

	this->terminal = terSet;
	this->dfaNodeTable = Dtran;

}


//////////////此处效率太低，需要改进
bool DFA::isTerminate(set<int> U, vector<int> &terSet, int &terNum)//判断结点集U中是否含有终止结点
{
	terNum = 0;
	terSet.clear();
	set<int>::iterator  iter2;
	bool ister = false;
	for (iter2 = U.begin(); iter2 != U.end(); iter2++)
	{
		if (this->isAcc[*iter2] == 1)
		{
			terNum++;
			terSet.push_back(*iter2);
			ister = true;
		}

	}

	/*	for(iter1=acceptedNode.begin();iter1<acceptedNode.end();iter1++)
		{
			//cout<<(*iter1).id<<"---------"<<*iter2<<endl;
			if((*iter1).id==*iter2)
			{
				type=iter1->type;
				ister=true;

				iter1=acceptedNode.end();
				iter2=U.end();
				iter2--;
			}

	/*
	for(iter2=U.begin();iter2!=U.end();iter2++)
	{
		map<int,string>::iterator iter;
		iter=this->terminal.find(*iter2);
		if(iter!=terminal.end())
		{
			terNum++;
		}

	}*/

	return ister;

}
set<int> DFA::move(set<int> T, char a)
{
	set<int> target;//保存T中状态经过a能到达的状态集合
	set<int>::iterator iter;
	int edgeNum;
	dfaEdge tempEdge;//暂存边
	for (iter = T.begin(); iter != T.end(); iter++)//遍历状态集T，找出字母a的后继结点
	{
		int nodeNum = *iter;
		edgeNum = dfaNodeTable[nodeNum].edges.size();//该节点后继边的数目
		for (int i = 0; i < edgeNum; i++)
		{
			tempEdge = dfaNodeTable[nodeNum].edges[i];
			//int next=tempEdge.nextNode;//下一个结点标号
			//char symbol=tempEdge.symbol;
			if (tempEdge.symbol == a)//若当前边上symbol是a，将下一个结点标号加入到集合target
				target.insert(tempEdge.nextNode);
		}
	}
	return target;
}
set<int> DFA::epslonClosure(set<int> T)//求一个状态集合的epslon闭包
{
	set<int> closure;
	closure = T;//先将T中的元素全加入closure中
	int edgeNum;//=dfaNodeTable[s].edges.size();
	dfaEdge tempEdge;//暂存边
	deque<int> nodeQue;//存未搜索的结点
	set<int>::iterator it;

	for (it = T.begin(); it != T.end(); it++)//将T中的所有元素加入队列nodeQue中
		nodeQue.push_back(*it);

	while (!nodeQue.empty())//当未搜索的结点队列不为空时
	{
		int nodeNum = nodeQue.front();//取队列前端的结点
		nodeQue.pop_front();        //将取出的结点从队列中删除
		closure.insert(nodeNum);
		edgeNum = dfaNodeTable[nodeNum].edges.size();
		for (int i = 0; i < edgeNum; i++)
		{
			tempEdge = dfaNodeTable[nodeNum].edges[i];
			int next = tempEdge.nextNode;//下一个结点标号
			if (tempEdge.symbol == '@' && !isIn(closure, next))//若当前边上symbol是epslon，且下一个结点标号不在closure中
			{
				closure.insert(next);
				nodeQue.push_back(next);
			}
		}
	}

	return closure;
}

set<int> DFA::epslonClosure(int s)//求一个结点的epslon闭包
{
	set<int> closure;
	closure.insert(s);
	int edgeNum;//=dfaNodeTable[s].edges.size();
	dfaEdge tempEdge;//暂存边
	deque<int> nodeQue;//存未搜索的结点
/*	for(int i=0;i<edgeNum;i++)
	{
		tempEdge=dfaNodeTable[s].edges[i];
		int next=tempEdge.nextNode;//下一个结点标号
		if(tempEdge.symbol=='@' && !isIn(closure,next) )//若当前边上symbol是epslon，且下一个结点标号不在closure中
		{
			closure.insert(next);
			nodeQue.push_back(next);

		}

	}*/

	nodeQue.push_back(s);
	while (!nodeQue.empty())//当未搜索的结点队列不为空时
	{
		int nodeNum = nodeQue.front();//取队列前端的结点
		nodeQue.pop_front();        //将取出的结点从队列中删除
		closure.insert(nodeNum);
		edgeNum = dfaNodeTable[nodeNum].edges.size();
		for (int i = 0; i < edgeNum; i++)//搜索当前结点的所有边
		{
			tempEdge = dfaNodeTable[nodeNum].edges[i];
			int next = tempEdge.nextNode;//下一个结点标号
			if (tempEdge.symbol == '@' && !isIn(closure, next))//若当前边上symbol是epslon，且下一个结点标号不在closure中
			{
				closure.insert(next);
				nodeQue.push_back(next);
			}

		}

	}

	//打印epslonClosure中的元素

	/*
	set<int>::iterator iter;
	for(iter=closure.begin();iter!=closure.end();iter++)
		cout<<*iter<<"##############"<<endl;
	 */

	return closure;

}
bool DFA::isIn(set<int> s, int element)
{
	set<int>::iterator it = s.find(element);
	if (it != s.end())
		return true;
	else
		return false;
}
void DFA::sortNodes()//
{

	int size = dfaNodeTable.size();
	vector<dfaNode> temp(size + 1);//暂存排好序的结点
	for (int i = 0; i < size; i++)
	{
		int index = dfaNodeTable[i].label;
		temp[index] = dfaNodeTable[i];
		isAcc[index] = dfaNodeTable[i].acceptstatetag;
	}
	dfaNodeTable = temp;

}

ostream &operator<<(ostream& output, const vector<dfaEdge> edges)
{
	vector<dfaEdge>::const_iterator iter;
	cout << "%%%%%%%%%%%" << endl;
	for (iter = edges.begin(); iter != edges.end(); iter++)
	{
		output << (*iter).nextNode << " ***** " << (*iter).symbol << endl;
	}
	return output;
}


void DFA::printNode()
{
	vector<dfaNode>::iterator iter;
	vector<dfaEdge>::const_iterator it;
	cout << "\n输出结点开始\n";
	for (iter = dfaNodeTable.begin(); iter != dfaNodeTable.end(); iter++)
	{
		cout << "LABEL  " << (*iter).label;
		cout << "边集" << (*iter).edges.size();
		if ((*iter).acceptstatetag == 1)
			cout << "终止" << endl;
		else
			cout << "非终止" << endl;

		for (it = (*iter).edges.begin(); it != (*iter).edges.end(); it++)
		{
			cout << (*it).nextNode << " ***** " << (*it).symbol << endl;
		}
	}

	//打印终止结点与其对应的终止类型
	map<int, string>::iterator it1;
	for (it1 = this->terminal.begin(); it1 != this->terminal.end(); it1++)
	{
		cout << it1->first << "..........." << it1->second << endl;

	}
}

bool DFA::getTerType(int num, string & str)
{
	map<int, string>::iterator iter;
	bool type = false;
	iter = this->terminal.find(num);
	if (iter != terminal.end())
	{
		str = iter->second;
		type = true;
	}
	/*for(iter=acceptedNode.begin();iter!=acceptedNode.end();iter++)
		if(iter->id == num)
		{
			str=iter->type;
			type=true;
			break;
		}
	*/
	return type;
}
