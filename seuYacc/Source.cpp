#include"Declaration.h"
extern vector<vector<string>> AnalyTable;//LR分析表
extern vector<GOTO> gotoTable;//所有goto
extern int parsingFile(const string& fileName);
extern void mapFirst();
extern void GenLRTable();
extern void InitTable();
extern void SetTable();
extern void genFile();
extern vector<LRState> stateTable;//所有状态
extern uniProduction uni_production;//所有产生式
int main(int argc, char* argv[])
{
	string thefile;
	//if (argc == 1) {
	//	cerr << "无yacc源文件" << endl;
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
	//写入LR分析表
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

	cout << "已生成语法分析器y.tab.cpp,y.tab.h" << endl;
	return 0;
}
