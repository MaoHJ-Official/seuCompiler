#include"Declaration.h"

extern vector<LRState> stateTable;//����״̬
extern vector<GOTO> gotoTable;//����goto
extern vector<string> pdnLeft;//���ս��
extern vector<string> tokenVector;//�ս��
vector<vector<string>> AnalyTable;//LR������
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
			if (!(*iteral).front().compare((*iter).mid))//�ƽ���goto,��"s"
			{
				(*iteral)[(*iter).left.stateCount + 1] = "s" + to_string((*iter).right.stateCount);
			}
		}

	}
	for (auto iter = stateTable.begin(); iter != stateTable.end(); ++iter)//����״̬
	{
		for (auto iteral = (*iter).item.begin(); iteral != (*iter).item.end(); ++iteral)//������
		{
			if ((*iteral).point == (*iteral).pdn.second.size())//���λ�������
			{
				for (auto itera = AnalyTable.begin(); itera != AnalyTable.end(); ++itera)//������ͷ
				{
					for (auto ite = (*iteral).predictSymbol.begin(); ite != (*iteral).predictSymbol.end(); ++ite)//����Ԥ���
					{
						if (!(*itera).front().compare((*ite)))//��ͷ==Ԥ���
						{
							if ((*iteral).pdn.first.compare(startExplus))//����ʽ�ұ�+#+���
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