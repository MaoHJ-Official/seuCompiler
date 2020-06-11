#include"Declaration.h"

extern vector<LRState> stateTable;//所有状态
extern vector<GOTO> gotoTable;//所有goto
extern vector<string> pdnLeft;//非终结符
extern vector<string> tokenVector;//终结符
vector<vector<string>> AnalyTable;//LR分析表
extern string startExplus;//S'
extern int Counts;
void InitTable()
{
	for (auto iteral = tokenVector.begin(); iteral != tokenVector.end(); ++iteral)
	{
		vector<string> temp(Counts + 2);
		temp[0] = (*iteral);
		AnalyTable.push_back(temp);
	}
	vector<string> temp(Counts + 2);
	temp[0] = "$";
	AnalyTable.push_back(temp);
	for (auto iteral = pdnLeft.begin(); iteral != pdnLeft.end(); ++iteral)
	{
		vector<string> temp(Counts + 2);
		temp[0] = (*iteral);
		AnalyTable.push_back(temp);
	}
}

void SetTable()
{
	for (auto iteral = AnalyTable.begin(); iteral != AnalyTable.end(); ++iteral)
	{
		for (auto iter = gotoTable.begin(); iter != gotoTable.end(); ++iter)
		{
			if (!(*iteral).front().compare((*iter).mid))//移进和goto,加"s"
			{
				(*iteral)[(*iter).left.stateCount + 1] = "s" + to_string((*iter).right.stateCount);
			}
		}

	}
	for (auto iter = stateTable.begin(); iter != stateTable.end(); ++iter)//遍历状态
	{
		for (auto iteral = (*iter).item.begin(); iteral != (*iter).item.end(); ++iteral)//遍历项
		{
			if ((*iteral).point == (*iteral).pdn.second.size())//点的位置在最后
			{
				for (auto itera = AnalyTable.begin(); itera != AnalyTable.end(); ++itera)//遍历表头
				{
					for (auto ite = (*iteral).predictSymbol.begin(); ite != (*iteral).predictSymbol.end(); ++ite)//遍历预测符
					{
						if (!(*itera).front().compare((*ite)))//表头==预测符
						{
							if ((*iteral).pdn.first.compare(startExplus))//产生式右边+#+左边
								(*itera)[(*iter).stateCount + 1] = to_string((*iteral).pdn.second.size()) + "#" + (*iteral).pdn.first;
							else
								(*itera)[(*iter).stateCount + 1] = "accept";
						}

					}
				}
			}
		}
	}
}