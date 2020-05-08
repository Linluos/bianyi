#include "SyntaxTree.h"

void SNLCompiler()
{
	///***************************************************************************************
	//词法分析
	//LexicalAnalysis();


	//语法分析预处理，从文件中读取产生式，构造产生式list
	string str("D:/Documents/bianyi/experiment/LL1/CompilerData/productions_new.txt");
	productionList* ptrProducList = new productionList(str);


	//语法分析中的构造LL1分析表
		//采用手动构造的方式，构造LL1分析表(并没有用到产生式list)
		//CreatLL1Table();
		//采用自动构造的方式，根据产生式，生成First集，Follow集，最后求出Predict集，以此构造LL1分析表
	//AutoMakeLL1Table(ptrProducList);
	ptrProducList->storeNonTerminalSet();
	ptrProducList->storeTerminalSet();
	ptrProducList->storeFirstMap();
	ptrProducList->storeFollowMap();
	ptrProducList->storePredictMap();

	//语法分析中的匹配
		//构造104个产生式对应的处理函数，手动匹配(因此并没有用到产生式list)，由于是用104个处理函数手动匹配，所以并没有匹配过程，直接输出语法树
		//ParseLL();
		//依据产生式，自动进行匹配(因此用到产生式list，参数为指向产生式list的指针)
	//SyntaxAnalysis(ptrProducList);


	//语法分析自动匹配构造词法分析树
	//CreateSyntaxTree(ptrProducList);
	//***************************************************************************************/


}
