#include"Declaration.h"
extern vector<vector<string>> AnalyTable;//LR������
extern vector<GOTO> gotoTable;//����goto
extern int parsingFile(const string& fileName);
extern void mapFirst();
extern void GenLRTable();
extern void InitTable();
extern void SetTable();
extern void genFile();
extern vector<LRState> stateTable;//����״̬
extern uniProduction uni_production;//���в���ʽ
int main(int argc, char* argv[])
{
	string thefile;
	//if (argc == 1) {
	//	cerr << "��yaccԴ�ļ�" << endl;
	//	exit(1);
	//}
	//else {
	//	thefile = argv[1];
	//}
	//parsingFile(thefile);
	parsingFile("yacc.y");
	mapFirst();
	GenLRTable();
	InitTable();
	SetTable();
	genFile();
	ofstream analy;
	analy.open("AnalyTable.txt", ios::out);
	cout << endl;
	////
	//д��LR������
	analy << AnalyTable.front().size() << endl;
	analy << AnalyTable.size() << endl;
	for (auto iteral = AnalyTable.begin(); iteral != AnalyTable.end(); ++iteral)
	{
		for (auto itera = iteral->begin(); itera != iteral->end(); ++itera)
		{
			analy << *itera << endl;
		}
	}
	analy.close();
	///
	cout << stateTable.begin()->item.size() << "  |  " << AnalyTable.front().size() << endl;
	cout << stateTable.size() << "  sssssss  " << endl;
	for (auto iteral = gotoTable.begin(); iteral != gotoTable.end(); ++iteral)
	{
		cout << iteral->left.stateCount << " " << iteral->mid << " " << iteral->right.stateCount;
		cout << endl;
	}

	cout << "�������﷨������y.tab.cpp,y.tab.h" << endl;
	return 0;
}
