#include"DFA.h"
struct stateSet//��С��ʱ��һ��״̬��
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
	/***************������δ��뽫DFA״̬ת��Ϊ��0��ʼ��******************/
	int size = this->dfaNodeTable.size();
	int startId = dfaNodeTable[0].label;
	for (int m = 0; m < size; m++)
	{
		dfaNodeTable[m].label -= startId;//���ý���ż�ȥ��ʼֵ
		int edgeNum = dfaNodeTable[m].edges.size();
		for (int edgeCounter = 0; edgeCounter < edgeNum; edgeCounter++)//��������̽���ż�ȥ��ʼֵ
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
	//this->printNode();/////////��ӡ���ڵ�����������á�������������





 //	map<set<int>,int> idTab;//״̬�������ŵĶ�Ӧ��

	 //map<int,int> numTab;//״̬����������ڲ�״̬���Ķ�Ӧ��

	vector<stateSet> devide;//��ֻ��ı�
	map<string, set<int>*> terType;//��ֹ������������Ӧ��״̬��ָ��
	stateSet tempSet;
	vector<int> stateToSet(size, -1);//״̬��ź�����״̬���ŵĶ�Ӧ��


	int setId = dfaNodeTable[size - 1].label + 1;//��ʼ��״̬����ID


	/*****�����ǽ�����״̬����Ϊ����ֹ��㼯����ֹ��㼯����ֹ���Ҫ����ֹ���ͻ���Ϊ���****/
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
	for (it2 = isAcc.begin(); it2 != isAcc.end(); it2++)//�ҳ���ʼ�ķ���ֹ״̬��
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
	devide.push_back(tempSet);//����ʼ�ķ���ֹ״̬������ֻ���
 //   idTab[tempSet.states]=setId++;//״̬�������ŵĶ�Ӧ��

	map<string, set<int>*>::iterator it3;
	for (it3 = terType.begin(); it3 != terType.end(); it3++)//����ʼ����ֹ״̬������ֻ���
	{
		tempSet.id = setId;
		tempSet.isTer = 1;
		//unTer=
		tempSet.states = *(it3->second);/////////////////////////////����������������������//////////////////////////////////�˴�������
		devide.push_back(tempSet);
		set<int>::iterator  riter;
		for (riter = tempSet.states.begin(); riter != tempSet.states.end(); riter++)
		{
			stateToSet[*riter] = setId;//����ֹ״̬����״̬��״̬����Ӧ��
		}
		setId++;
		//		idTab[tempSet.states]=setId++;//����״̬�������ŵĶ�Ӧ��
	}

	int beforeNum = 0;//һ�ηֻ�ǰ��״̬����
	int afterNum = devide.size();//һ�ηֻ����״̬����

	//vector<stateSet> newSet;//ɨ��һ��ֻ��������µ�״̬��

	set<int>::iterator qit;


	for (int k = 0; k < afterNum; k++)
	{
		cout << "��" << k << "���ֻ�   id    " << devide[k].id << '\n';

		for (qit = devide[k].states.begin(); qit != devide[k].states.end(); qit++)
			cout << *qit << "    ";
		cout << endl;

	}

	cout << "�ֻ���Ϊ*********" << afterNum << endl;

	for (int j = 0; j < stateToSet.size(); j++)//����ӵ�����㼯��
	{
		cout << "���" << j << "  ���� " << stateToSet[j] << endl;
	}






	map<int, set<int>*> setTab;
	set<int> Set;
	set<char>::iterator alphIter;//ɨ�����������ĸ�ı�����

	/************�������㷨�ĺ��Ĳ���*************/
	while (beforeNum != afterNum)//û���µ�״̬������ʱ��ѭ����ֹ
	{
		beforeNum = afterNum;
		//	newSet.clear();
		for (int i = 0; i < beforeNum; i++)//ɨ��һ�����״̬��
		{
			tempSet = devide[i];
			set<int>::iterator stateIter;//ɨ�����״̬�ı�����


			for (alphIter = charSet.begin(); alphIter != charSet.end(); alphIter++)
			{ //�����ĸ����ÿ���������,��״̬����ÿ��״̬ɨ��һ�飬�ж����̽�����ڼ���
				//cout<<*alphIter<<endl;
				Set.clear();
				map<int, set<int>*>::iterator deIter;
				for (deIter = setTab.begin(); deIter != setTab.end(); deIter++)//�ͷŵ���̬�ڴ�
				{
					delete deIter->second;
				}
				setTab.clear();
				int idOfSet = -1;
				for (stateIter = tempSet.states.begin(); stateIter != tempSet.states.end(); stateIter++)
				{//��ʼ��״̬����ÿ��״̬��ɨ��
					int stateId = *stateIter;
					char letter = *alphIter;//�ݴ����ĸ
					int nextId = dfaNodeTable[stateId].nextId(letter);//��ú�̽���ID���

					if (nextId != -1)//��̽�����
						idOfSet = stateToSet[nextId];
					else
						idOfSet = -1;
					if (!isIn(Set, idOfSet))
					{
						Set.insert(idOfSet);//����״̬����
						setTab[idOfSet] = new set<int>;
					}
					setTab[idOfSet]->insert(stateId);

				}//������״̬����ÿ��״̬��ɨ��

				int sizeOfSet = Set.size();
				if (sizeOfSet > 1)//�������ɵ�״̬������ֻ�
				{
					stateSet temp;
					temp.id = tempSet.id;
					temp.states = *(setTab.begin()->second);
					devide[i] = temp;//����ɨ���״̬���û�
					tempSet = temp;
					map<int, set<int>*>::iterator siter;
					set<int>::iterator qiter;
					for (siter = ++setTab.begin(); siter != setTab.end(); siter++)
					{   //�������ɵ�״̬������ֻ���
						temp.id = setId++;
						temp.states = *(siter->second);
						for (qiter = temp.states.begin(); qiter != temp.states.end(); qiter++)
							stateToSet[*qiter] = temp.id;
						devide.push_back(temp);
					}
					//break;
				}//�����ɵ�״̬������ֻ�����	


			}//������ÿ��������ĸ��ɨ��

		}//������ÿ��״̬����ɨ��

		afterNum = devide.size();
	}

	cout << "��С����һ�����!!!\n";
	cout << "�ֻ���Ϊ*********" << afterNum << endl;

	for (int j = 0; j < stateToSet.size(); j++)//������ݷֻ�������dfa����
	{
		cout << "���" << j << "  ���� " << stateToSet[j] << endl;
	}

	for (int k = 0; k < afterNum; k++)
	{
		cout << "��" << k << "���ֻ�\n";
		set<int>::iterator qit;
		for (qit = devide[k].states.begin(); qit != devide[k].states.end(); qit++)
			cout << *qit << "    ";
		cout << endl;

	}


	cout << "������ݷֻ���ʼ������С����DFA��\n";

	for (it2 = isAcc.begin(); it2 != isAcc.end(); it2++)//
	{
		cout << "���   " << it2->first << "      " << it2->second << endl;
	}


	cout << "������ֹ����\n";
	for (it1 = this->terminal.begin(); it1 != this->terminal.end(); it1++)
	{
		cout << "���   " << it1->first << "      " << it1->second << endl;
	}


	vector<dfaNode> tempdfa;
	dfaNode tempNode;
	int terNumber = 0;
	string accType;
	map<int, string> tempTerType;


	cout << "�������ACC���\n";
	map<int, int>::iterator mpit;
	for (mpit = this->isAcc.begin(); mpit != this->isAcc.end(); mpit++)
		cout << mpit->first << "  is  " << mpit->second << endl;


	set<string> terTemp;
	for (int index = 0; index < afterNum; index++)//ɨ��ֻ�
	{
		terTemp.clear();
		for (qit = devide[index].states.begin(); qit != devide[index].states.end(); qit++)
		{//ɨ��ÿ��״̬�����ҳ���ʼ״̬��������¼������ֹ������ֹ����
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
	//����������С��DFA�ı�
	for (int index = 0; index < afterNum; index++)//ɨ�����ɵĽ��
	{
		qit = devide[index].states.begin();
		int stateID = *qit;//�ӽ�㼯��ȥһ������
		int nextID = -1;
		for (alphIter = charSet.begin(); alphIter != charSet.end(); alphIter++)
		{
			nextID = this->dfaNodeTable[stateID].nextId(*alphIter);//�������ĸ��Ӧ����һ�������
			if (nextID != -1)//���ں�̽��
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
void DFA::subsetCon()//�����Ӽ����취��NFA����DFA
{
	map<set<int>, int > Dstates;//�������ɵ�DFA���
	map<int, set<int> > states;
	deque< int > unmarkStates;//����״̬�е�δ���״̬

	set<int> T = epslonClosure(start);//��ʼ״̬��epslon�հ�
	int nodeId = this->dfaNodeTable.size();//Ϊ�˱��������ɵĽ��ed��ԭ�����id�ĳ�ͻ
	set<int> tempClosure;    //�ݴ����ɵ�epslon�հ�
	Dstates[T] = nodeId;       //����ʼ״̬����Dstates
	unmarkStates.push_back(nodeId);//��δ���״̬���м����״̬
	states[nodeId] = T;

	vector<dfaNode> Dtran;//�����ɵ�DFA��*******��Ҫ������С��ʱҪ��
	//dfaNode * nodePtr=new dfaNode();
	//nodePtr->label=nodeId;
	//nodePtr->acceptstatetag=0;
	dfaNode* nodePtr = NULL;
	string terType;//��ֹ��㼯����

	map<int, string> terSet;//����ֹ��㼯
	TerminalNode * terPtr = NULL;//ָ�������ɵ���ֹ���
	int number = 0; //ÿ����ֹ��㼯����ֹ״̬����

	while (!unmarkStates.empty())//
	{
		int unmark = unmarkStates.front();//ȡ��һ��δ��ǵĽ��
		unmarkStates.pop_front();//���ý���δ��Ƕ����е���

	   /*
		dfaEdge tempEdge;
		tempEdge.symbol=*iter;
		tempEdge.nextNode=nodeId;
		nodePtr->edges.push_back(tempEdge);*/

		nodePtr = new dfaNode(unmark); //�����½��
		vector<int> tempTer;
		if (isTerminate(states[unmark], tempTer, number))//����ǰδ��ǵ�״̬���к�����ֹ���
		{

			nodePtr->acceptstatetag = 1;//�򵯳���״̬����Ӧ���Ϊ��ֹ̬
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

			terSet[unmark] = terType;//����ֹ��㼯���м���ý��

		}


		set<char>::iterator iter;
		for (iter = charSet.begin(); iter != charSet.end(); iter++)//����ĸ���е�ÿ���������
		{
			T = states[unmark];//ȡ��δ��ǵ�״̬��
			tempClosure = move(T, *iter);
			if (!tempClosure.empty())//��㼯���б�ŵ�ǰ��ĸ�ķ�����
			{
				tempClosure = this->epslonClosure(move(T, *iter));//����epslon�հ�
				map<set<int>, int>::iterator it = Dstates.find(tempClosure);
				if (it == Dstates.end())//�����ɵ�״̬������Dstates��
				{
					nodeId++;
					Dstates[tempClosure] = nodeId;//�������ɵ�״̬����Dstates��
					states[nodeId] = tempClosure;
					unmarkStates.push_back(nodeId);//�������ɵ�״̬����δ���״̬����

					dfaEdge tempEdge;
					tempEdge.symbol = *iter;
					tempEdge.nextNode = nodeId;
					nodePtr->edges.push_back(tempEdge);//�������ɱ߼�����߼���

				}
				else//�����ɵ�״̬����Dstates��
				{
					dfaEdge tempEdge;
					tempEdge.symbol = *iter;
					tempEdge.nextNode = Dstates[tempClosure];
					nodePtr->edges.push_back(tempEdge);//�������ɱ߼�����߼���
				}


			}

		}//��ĸ����������������

		Dtran.push_back(*nodePtr);//���ձ�ǵĽ�����DFA����
		delete nodePtr;//�ͷ���ʱ���ɵĽ��
	}

	vector<dfaNode>::iterator nodeiter;

	cout << endl << endl;
	this->isAcc.clear();
	//��ʱע�͵�
	//cout<<"ȷ�������DFA����������\n";
	for (nodeiter = Dtran.begin(); nodeiter != Dtran.end(); nodeiter++)
	{

		isAcc[nodeiter->label] = nodeiter->acceptstatetag;//������ֹ����

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


//////////////�˴�Ч��̫�ͣ���Ҫ�Ľ�
bool DFA::isTerminate(set<int> U, vector<int> &terSet, int &terNum)//�жϽ�㼯U���Ƿ�����ֹ���
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
	set<int> target;//����T��״̬����a�ܵ����״̬����
	set<int>::iterator iter;
	int edgeNum;
	dfaEdge tempEdge;//�ݴ��
	for (iter = T.begin(); iter != T.end(); iter++)//����״̬��T���ҳ���ĸa�ĺ�̽��
	{
		int nodeNum = *iter;
		edgeNum = dfaNodeTable[nodeNum].edges.size();//�ýڵ��̱ߵ���Ŀ
		for (int i = 0; i < edgeNum; i++)
		{
			tempEdge = dfaNodeTable[nodeNum].edges[i];
			//int next=tempEdge.nextNode;//��һ�������
			//char symbol=tempEdge.symbol;
			if (tempEdge.symbol == a)//����ǰ����symbol��a������һ������ż��뵽����target
				target.insert(tempEdge.nextNode);
		}
	}
	return target;
}
set<int> DFA::epslonClosure(set<int> T)//��һ��״̬���ϵ�epslon�հ�
{
	set<int> closure;
	closure = T;//�Ƚ�T�е�Ԫ��ȫ����closure��
	int edgeNum;//=dfaNodeTable[s].edges.size();
	dfaEdge tempEdge;//�ݴ��
	deque<int> nodeQue;//��δ�����Ľ��
	set<int>::iterator it;

	for (it = T.begin(); it != T.end(); it++)//��T�е�����Ԫ�ؼ������nodeQue��
		nodeQue.push_back(*it);

	while (!nodeQue.empty())//��δ�����Ľ����в�Ϊ��ʱ
	{
		int nodeNum = nodeQue.front();//ȡ����ǰ�˵Ľ��
		nodeQue.pop_front();        //��ȡ���Ľ��Ӷ�����ɾ��
		closure.insert(nodeNum);
		edgeNum = dfaNodeTable[nodeNum].edges.size();
		for (int i = 0; i < edgeNum; i++)
		{
			tempEdge = dfaNodeTable[nodeNum].edges[i];
			int next = tempEdge.nextNode;//��һ�������
			if (tempEdge.symbol == '@' && !isIn(closure, next))//����ǰ����symbol��epslon������һ������Ų���closure��
			{
				closure.insert(next);
				nodeQue.push_back(next);
			}
		}
	}

	return closure;
}

set<int> DFA::epslonClosure(int s)//��һ������epslon�հ�
{
	set<int> closure;
	closure.insert(s);
	int edgeNum;//=dfaNodeTable[s].edges.size();
	dfaEdge tempEdge;//�ݴ��
	deque<int> nodeQue;//��δ�����Ľ��
/*	for(int i=0;i<edgeNum;i++)
	{
		tempEdge=dfaNodeTable[s].edges[i];
		int next=tempEdge.nextNode;//��һ�������
		if(tempEdge.symbol=='@' && !isIn(closure,next) )//����ǰ����symbol��epslon������һ������Ų���closure��
		{
			closure.insert(next);
			nodeQue.push_back(next);

		}

	}*/

	nodeQue.push_back(s);
	while (!nodeQue.empty())//��δ�����Ľ����в�Ϊ��ʱ
	{
		int nodeNum = nodeQue.front();//ȡ����ǰ�˵Ľ��
		nodeQue.pop_front();        //��ȡ���Ľ��Ӷ�����ɾ��
		closure.insert(nodeNum);
		edgeNum = dfaNodeTable[nodeNum].edges.size();
		for (int i = 0; i < edgeNum; i++)//������ǰ�������б�
		{
			tempEdge = dfaNodeTable[nodeNum].edges[i];
			int next = tempEdge.nextNode;//��һ�������
			if (tempEdge.symbol == '@' && !isIn(closure, next))//����ǰ����symbol��epslon������һ������Ų���closure��
			{
				closure.insert(next);
				nodeQue.push_back(next);
			}

		}

	}

	//��ӡepslonClosure�е�Ԫ��

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
	vector<dfaNode> temp(size + 1);//�ݴ��ź���Ľ��
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
	cout << "\n�����㿪ʼ\n";
	for (iter = dfaNodeTable.begin(); iter != dfaNodeTable.end(); iter++)
	{
		cout << "LABEL  " << (*iter).label;
		cout << "�߼�" << (*iter).edges.size();
		if ((*iter).acceptstatetag == 1)
			cout << "��ֹ" << endl;
		else
			cout << "����ֹ" << endl;

		for (it = (*iter).edges.begin(); it != (*iter).edges.end(); it++)
		{
			cout << (*it).nextNode << " ***** " << (*it).symbol << endl;
		}
	}

	//��ӡ��ֹ��������Ӧ����ֹ����
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
