#include "SyntaxTree.h"

void SNLCompiler()
{
	///***************************************************************************************
	//�ʷ�����
	//LexicalAnalysis();


	//�﷨����Ԥ�������ļ��ж�ȡ����ʽ���������ʽlist
	string str("D:/Documents/bianyi/experiment/LL1/CompilerData/productions_new.txt");
	productionList* ptrProducList = new productionList(str);


	//�﷨�����еĹ���LL1������
		//�����ֶ�����ķ�ʽ������LL1������(��û���õ�����ʽlist)
		//CreatLL1Table();
		//�����Զ�����ķ�ʽ�����ݲ���ʽ������First����Follow����������Predict�����Դ˹���LL1������
	//AutoMakeLL1Table(ptrProducList);
	ptrProducList->storeNonTerminalSet();
	ptrProducList->storeTerminalSet();
	ptrProducList->storeFirstMap();
	ptrProducList->storeFollowMap();
	ptrProducList->storePredictMap();

	//�﷨�����е�ƥ��
		//����104������ʽ��Ӧ�Ĵ��������ֶ�ƥ��(��˲�û���õ�����ʽlist)����������104���������ֶ�ƥ�䣬���Բ�û��ƥ����̣�ֱ������﷨��
		//ParseLL();
		//���ݲ���ʽ���Զ�����ƥ��(����õ�����ʽlist������Ϊָ�����ʽlist��ָ��)
	//SyntaxAnalysis(ptrProducList);


	//�﷨�����Զ�ƥ�乹��ʷ�������
	//CreateSyntaxTree(ptrProducList);
	//***************************************************************************************/


}
