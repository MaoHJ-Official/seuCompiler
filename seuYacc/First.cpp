#include"Declaration.h"

extern uniProduction uni_production;//所有产生式
FirstMap firstMap;//所有计算出的符号first集
extern vector<string> tokenVector;//终结符
bool judgeToken(vector<string>& str, string val)
{
	//cout << "c" << endl;
	for (auto iteral = str.begin(); iteral != str.end(); ++iteral)
	{
		if (!val.compare(*iteral))
			return true;
	}
	return false;
}

void calcFirst(string symbol, set<string>& temp)
{
	//cout << "d" << endl;
	for (auto iteral = uni_production.begin(); iteral != uni_production.end(); ++iteral)
	{
		if (!iteral->first.compare(symbol))
		{
			if (judgeToken(tokenVector, (*iteral).second.front()))//终结符
			{
				//cout << "E" << endl;
				temp.insert((*iteral).second.front());
			}

			else//非终结符
			{
				//cout << "F" << endl;

				if (temp.find((*iteral).second.front()) != temp.end())
					break;
				temp.insert((*iteral).second.front());
				cout << "F" << endl;
				calcFirst((*iteral).second.front(), temp);
			}
		}
	}
}

void mapFirst()
{
	cout << "mapFirst()" << endl;
	for (auto iteral = uni_production.begin(); iteral != uni_production.end(); ++iteral)
	{
		set<string> temp;
		calcFirst((*iteral).first, temp);
		pair<string, set<string>> pa;
		pa.first = iteral->first;
		pa.second = temp;
		firstMap.insert(pa);
	}
	for (auto iteral = tokenVector.begin(); iteral != tokenVector.end(); ++iteral)
	{
		set<string> temp;
		pair<string, set<string>> pa;
		pa.first = *iteral;
		temp.insert(*iteral);
		pa.second = temp;
		firstMap.insert(pa);
	}
	cout << "firstMapSize:" << firstMap.size() << endl;
}

