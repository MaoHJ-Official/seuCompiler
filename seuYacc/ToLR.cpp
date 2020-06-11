#include"Declaration.h"

vector<GOTO> gotoTable;//所有goto
bool StateCompare(LRState state);
bool PredictCompare(vector<string> v1, vector<string> v2);
bool itemscmp(vector<LRItem> items1, vector<LRItem> items2);
bool ItemCompare(LRItem item1, LRItem item2);
bool judgeToken(vector<string>& str, string val);
extern uniProduction uni_production;//所有产生式
extern vector<string> tokenVector;//终结符
vector<LRState> stateTable;//所有状态
extern vector<string> pdnLeft;//非终结符
int Counts = 0;//计状态数
string startExplus = "startExplus";//S'
extern FirstMap firstMap;//所有计算出的符号first集
extern string startExp;//开始符号
LRState StateFind(LRState state);
bool inStateTable(vector<LRItem> items1, LRItem item);
bool gotoCompare(GOTO got);
int genCount()
{
	return ++Counts;
}

void Closure(LRState& state)
{

	int initSize;
	do
	{
		initSize = 0;

		//cout << initSize << " ";
		for (auto iteral = state.item.begin(); iteral != state.item.end(); ++iteral)
		{
			int tet = iteral - state.item.begin();
			if ((*iteral).point != (*iteral).pdn.second.size() && !judgeToken(tokenVector, (*iteral).pdn.second[(*iteral).point]))
			{
				for (auto temp = uni_production.begin(); temp != uni_production.end(); ++temp)
				{
					if ((*iteral).point != (*iteral).pdn.second.size() && !iteral->pdn.second[(*iteral).point].compare((*temp).first))
					{
						LRItem test;
						if ((*iteral).point == (*iteral).pdn.second.size() - 1)
							test.predictSymbol = iteral->predictSymbol;

						else
						{
							for (auto tmp = firstMap[(*iteral).pdn.second[(*iteral).point + 1]].begin();
								tmp != firstMap[(*iteral).pdn.second[(*iteral).point + 1]].end(); ++tmp)
							{
								test.predictSymbol.push_back((*tmp));
							}
						}
						test.point = 0;
						test.pdn = (*temp);
						if (!inStateTable(state.item, test))
						{
							state.item.push_back(test);
							initSize++;
							iteral = state.item.begin() + tet;
							//cout << "Statess size: "<<state.item.size() << endl;
						}

					}
				}

			}
		}
	} while (initSize > 0);

}

LRState GOTOLR(LRState state, string temp)
{
	//cout << "22" << endl;
	LRState Next;
	Next.stateCount = genCount();
	for (auto iteral = state.item.begin(); iteral != state.item.end(); ++iteral)
	{
		if (iteral->pdn.second.size() != iteral->point)
		{
			if (!iteral->pdn.second[iteral->point].compare(temp))
			{
				LRItem tmp;
				tmp.pdn = (*iteral).pdn;
				tmp.point = (*iteral).point + 1;
				tmp.predictSymbol = (*iteral).predictSymbol;
				Next.item.push_back(tmp);
			}

		}
	}
	Closure(Next);

	return Next;
}

void GenLRTable()
{
	//cout << "12" << endl;
	int initSize;
	LRItem item;
	item.point = 0;
	item.predictSymbol.push_back("$");
	item.pdn.first = startExplus;
	item.pdn.second.push_back(startExp);
	LRState temp;
	temp.stateCount = 0;
	temp.item.push_back(item);
	Closure(temp);
	stateTable.push_back(temp);
	do
	{

		initSize = 0;
		//cout << initSize << " ";
		for (auto iteral = stateTable.begin(); iteral != stateTable.end(); ++iteral)
		{
			int tet = iteral - stateTable.begin();
			for (auto it1 = pdnLeft.begin(); it1 != pdnLeft.end(); ++it1)//非终结符
			{
				LRState tem = GOTOLR((*iteral), (*it1));
				if (tem.item.size() != 0)
				{
					if (!StateCompare(tem))
					{
						GOTO got;
						got.left = (*iteral);
						got.mid = (*it1);
						got.right = tem;
						gotoTable.push_back(got);
						stateTable.push_back(tem);
						iteral = stateTable.begin() + tet;
						initSize++;
						cout << initSize << endl;
					}
					else
					{

						Counts--;
						GOTO got;
						got.left = (*iteral);
						got.mid = (*it1);
						got.right = StateFind(tem);
						if (!gotoCompare(got))
							gotoTable.push_back(got);
					}

				}
				else
					Counts--;
			}
			for (auto it2 = tokenVector.begin(); it2 != tokenVector.end(); ++it2)//终结符
			{
				LRState tem = GOTOLR((*iteral), (*it2));
				if (tem.item.size() != 0)
				{

					if (!StateCompare(tem))
					{
						GOTO got;
						got.left = (*iteral);
						got.mid = (*it2);
						got.right = tem;
						gotoTable.push_back(got);
						stateTable.push_back(tem);
						iteral = stateTable.begin() + tet;
						initSize++;
					}
					else
					{
						Counts--;
						GOTO got;
						got.left = (*iteral);
						got.mid = (*it2);
						got.right = StateFind(tem);
						if (!gotoCompare(got))
							gotoTable.push_back(got);
					}

				}
				else
					Counts--;
			}
		}
	} while (initSize > 0);
}

bool StateCompare(LRState state)
{
	for (auto iteral = stateTable.begin(); iteral != stateTable.end(); ++iteral)
	{
		if (itemscmp(state.item, (*iteral).item))
			return true;
	}
	return false;
}

LRState StateFind(LRState state)
{
	//cout << "B";
	for (auto iteral = stateTable.begin(); iteral != stateTable.end(); ++iteral)
	{
		if (itemscmp(state.item, (*iteral).item))
		{
			//cout << "C\n";
			return (*iteral);
		}

	}

}

bool whethercontain(string& str, vector<string>& v1)
{
	for (auto iteral = v1.begin(); iteral != v1.end(); ++iteral)
	{
		if (!str.compare(*iteral))
			return true;
	}
	return false;
}
bool PredictCompare(vector<string> v1, vector<string> v2)
{
	if (v1.size() != v2.size())
		return false;
	for (auto iteral = v1.begin(); iteral != v1.end(); ++iteral)
	{
		if (!whethercontain((*iteral), v2))
			return false;
	}
	return true;
}

bool ItemCompare(LRItem item1, LRItem item2)
{
	if (item1.point != item2.point)
		return false;
	if (item1.pdn.first.compare(item2.pdn.first))
		return false;
	if (!PredictCompare(item1.predictSymbol, item2.predictSymbol))
		return false;
	if (!PredictCompare(item1.pdn.second, item2.pdn.second))
		return false;
	return true;
}



bool itemscmp(vector<LRItem> items1, vector<LRItem> items2)
{
	int f = 0;
	if (items1.size() != items2.size()) return false;
	for (auto iteral = items1.begin(); iteral != items1.end(); ++iteral)
	{
		for (auto itera = items2.begin(); itera != items2.end(); ++itera)
		{
			if (ItemCompare((*itera), (*iteral)))
			{
				f = 1;
				break;
			}
		}
		if (f == 0)
			return false;
		else f = 0;
	}
	return true;
}
bool inStateTable(vector<LRItem> items1, LRItem item)
{
	for (auto iteral = items1.begin(); iteral != items1.end(); ++iteral)
	{
		if (ItemCompare(*iteral, item))
			return true;
	}
	return false;
}

bool gotoCompare(GOTO got)
{
	for (auto iteral = gotoTable.begin(); iteral != gotoTable.end(); ++iteral)
	{
		if (iteral->left.stateCount == got.left.stateCount && !iteral->mid.compare(got.mid) && iteral->right.stateCount == got.right.stateCount)
			return true;
	}
	return false;
}