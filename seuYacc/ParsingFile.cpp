#include"Declaration.h"

vector<string> functionVector;//��������
vector<string> pdnLeft;//���ս��
vector<string> tokenVector;//�ս��
string startExp;//��ʼ����
uniProduction uni_production;//���в���ʽ
int parsingFile(const string& fileName)
{
	ifstream in;
	in.open(fileName, ios::in);
	string temp;//file buffer
	if (!in)
	{
		cout << "ERROR: No such a file or can't open it!" << endl;
		return -1;
	}
	in >> temp;

	if (temp == "%{") {
		while (!in.eof())
		{
			getline(in, temp);
			if (temp == "%}")break;
			functionVector.push_back(temp);
		}
		cout << "Error: no rule" << endl;
	}
	in >> temp;

	while (temp != "%left" && temp != "%start" && temp != "%%")
	{
		if (temp == "%token") {
			if (in.eof())cout << "Error: no rule" << endl;
			in >> temp;
		}
		//cout << temp << endl;
		tokenVector.push_back(temp);
		if (in.eof())cout << "Error: no rule" << endl;
		in >> temp;
	}

	while (temp != "%start" && temp != "%%")
	{
		if (temp == "%left") {
			if (in.eof())cout << "Error: no rule" << endl;
			in >> temp;
		}
		tokenVector.push_back(temp);
		if (in.eof())cout << "Error: no rule" << endl;
		in >> temp;
	}
	//	while (temp != "%%")
	//	{
	if (temp == "%start")
	{
		if (in.eof())cout << "Error: no rule" << endl;
		in >> temp;
		startExp = temp;
		if (in.eof())cout << "Error: no rule" << endl;
		in >> temp;
	}
	//	}
	if (temp == "%%")/*-----------------part2------------------------*/
	{
		if (in.eof())cout << "Error: no %% end" << endl;
		in >> temp;
		while (temp != "%%")
		{
			//vector<string> expRight;//����ʽ�Ҳ�
			Production production;//һ������ʽ
			production.first = temp;
			pdnLeft.push_back(temp);
			if (in.eof())cout << "Error: no %% end" << endl;
			in >> temp;
			if (temp != ":")
			{
				cout << "ERROR: No \":\"!" << endl;
				return -1;
			}
			//in >> temp;

			do
			{
				if (in.eof())cout << "Error: no %% end" << endl;
				in >> temp;
				while (temp != "|" && temp != ";")
				{
					production.second.push_back(temp);
					//cout << temp << endl;
					if (in.eof())cout << "Error: no %% end" << endl;
					in >> temp;
				}
				uni_production.push_back(production);
				production.second.clear();
			} while (temp != ";");
			if (in.eof())cout << "Error: no %% end" << endl;
			in >> temp;
		}
	}
	if (temp == "%%")/*-----------------part3------------------------*/
	{
		while (!in.eof())
		{
			getline(in, temp);
			functionVector.push_back(temp);
		}
	}
	return 0;
}