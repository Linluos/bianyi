#include"PredictSet.h"

productionList::productionList()//Ĭ�Ϲ��캯��
{

}

productionList::productionList(string dir)//����Ϊ����ʽ�ļ����ڵ�Ŀ¼·��+�ļ���
{
	//������ʽlist�ͷ��ռ���set
	readProducList(dir);

	//����ռ���set
	fillTerminalSet();

	//Ԥ�����First��map��Follow��map��Predict��map��keyֵ��������һ���Ĺ���
	InitialFirstFollowPredictMap();

	//����ÿ�����ռ�����first��
	establishFirstMap();

	//����ÿ�����ռ�����follow��
	establishFollowMap();

	//����ÿ�����ռ�����predict��
	establishPredictMap();

	//ʵ�ִ�string��enum����LexType��ӳ�䣬���㹹��LL1������
	//makeMapFromStringToLexType();

	//ʵ�ִ�string��enum����NontmlType��ӳ�䣬���㹹��LL1������
	//makeMapFromStringToNontmlType();

}
/*****************************************************************************************/
//������ʽlist�ͷ��ռ���set
/*****************************************************************************************/
void productionList::readProducList(string dir)
{
	ifstream infile;
	infile.open(dir);
	if (!infile)
	{
		cout << "ProducList�ļ���ʧ�ܣ�" << endl;
	}
	string line;//���ļ��ж�ȡһ�в���ʽ
	string substr;//���طָ����ַ���
	production* ptrProduc;//ָ��һ������ʽ
	bool equalFlag = false;//��־�Ƿ��Ѷ���"::="�������
	int seqNum;//��ǰ�кţ�Ҳ������ʽ�ı��


	while (getline(infile, line))
	{
		istringstream stream(line);//���ڷָ��ַ� ���������Ǵ�string����line�ж�ȡ�ַ���istringstream������ִ��C++���Ĵ��������������
		ptrProduc = new production();//һ�ж�Ӧһ������ʽ

		stream >> substr;//ÿ�еĵ�һ��Ԫ�رض����к�
		substr.pop_back();//����ɾ�����һ��Ԫ��.
		istringstream toInt(substr);
		toInt >> seqNum;
		ptrProduc->sequenceNum = seqNum;
		//cout << "Sequence:" << seqNum << endl;

		while (stream >> substr)
		{
			if (substr == "::=")
			{
				equalFlag = true;
				//cout << "::=" << endl;
				continue;//�ԵȺŲ����κδ���
			}

			if (!equalFlag)//û����::=
			{
				ptrProduc->productionLeft = substr;
				//cout << "Left:" << substr << endl;
				nonTerminalSet.insert(substr);//�Ӳ���ʽ��������ռ�����������ռ�����
			}
			else
			{
				ptrProduc->productionRight.push_back(substr);//�µ�Ԫ�ؼӵ�����棬λ��Ϊ��ǰ���һ��Ԫ�ص���һ��Ԫ��
				//cout << "Right:" << substr << endl;
			}
		}


		producList.push_back(*ptrProduc);//���������ʽ������producList��
		equalFlag = false;//һ�����ݽ���������־��λ
		line.clear();
		substr.clear();
		//cout <<"This line is over!\n\n\n\n"<< endl;
	}
	infile.close();
	
}
/*****************************************************************************************/
//����ռ�����������֮����˹�����String��LexType��ӳ��
/*****************************************************************************************/
void productionList::fillTerminalSet()
{
	list<production>::iterator iter;
	vector<string>::iterator it;
	for (iter = producList.begin(); iter != producList.end(); iter++)
	{
		for (it = (*iter).productionRight.begin(); it != (*iter).productionRight.end(); it++)
		{
			if (nonTerminalSet.find(*it) != nonTerminalSet.end())//���ķ������ڷ��ռ�������
			{

			}
			else//���ķ�����Ӧ�����ռ�������
			{
				TerminalSet.insert(*it);
			}
		}
	}
}

void productionList::InitialFirstFollowPredictMap()
{
	//һ���յ�string��set��
	set<string> nullstrset;

	//���firstmap��followmap
	set<string>::iterator iter;
	for (iter = nonTerminalSet.begin(); iter != nonTerminalSet.end(); iter++)
	{
		firstMap.insert(pair<string, set<string>>(*iter, nullstrset));//���ֵΪ1��key�����ڣ����(1,"one")���pair������
		followMap.insert(pair<string, set<string>>(*iter, nullstrset));//pairʵ������һ���ṹ�壬����Ҫ��������Ա������first��second 
	}

	//���ڿ�ʼ��S����"#"������follow����
	followMap.find(producList.begin()->productionLeft)->second.insert("#");//->second���ǳɹ����ı�־

	//���predictmap
	list<production>::iterator produciter;
	for (produciter = producList.begin(); produciter != producList.end(); produciter++)
	{
		//map��keyֵ���Զ������ͣ����ܼӽ�ȥ��ԭ����û�����رȽ��������û���Ƚϣ���Ȼû����insert����--------------------�ѽ��
		predictMap.insert(pair<production, set<string >>((*produciter), nullstrset));
	}

}

void productionList::establishFirstMap()
{
	for (int i = 0; i < 10; i++)//Ϊ��֤�պϣ���������
	{
		set<string>::iterator NonTermimalIter;//�������ռ������ϵĵ�����
		for (NonTermimalIter = nonTerminalSet.begin(); NonTermimalIter != nonTerminalSet.end(); NonTermimalIter++)
		{
			findFirstSet(*NonTermimalIter);
		}
	}


}


void productionList::findFirstSet(string nonTerStr)
{
	list<production>::iterator producIter;//��������ʽ����ĵ�����
	for (producIter = producList.begin(); producIter != producList.end(); producIter++)
	{
		if ((*producIter).productionLeft == nonTerStr)//�ҵ����������Ĳ���ʽ
		{
			vector<string>::iterator producRightIter;//�����ò���ʽ�Ҳ���ķ�����vector�ĵ�����
			for (producRightIter = (*producIter).productionRight.begin(); producRightIter != (*producIter).productionRight.end(); producRightIter++)//�����ò���ʽ���Ҳ�
			{
				if (nonTerminalSet.find(*producRightIter) != nonTerminalSet.end())//���ķ������Ƿ��ռ���
				{
					/**<-------------(firstMap.find(*producRightIter)->second)�Ǹò���ʽ�Ҳ��һ�����ռ�����Ӧ��firstSet------------------------>**/
					if ((firstMap.find(*producRightIter)->second).find("?") != (firstMap.find(*producRightIter)->second).end())//������ڸ÷��ռ�����first������,����Ҫ���������ң����Բ���break
					{
						if (*producRightIter == (*producIter).productionRight.back())//�����ķ������ǲ���ʽ���Ҳ�ķ��ռ������Ҹ��ռ��������Ƴ����գ���ô�������ʽ�����ķ��ռ�����first���бض��п�
						{
							(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
						}
						else//�����ķ����Ų��ǲ���ʽ���Ҳ�ķ��ռ������Ѹ��ķ����ŵ�first�����˿�֮���Ԫ�ؼ��뵽��ߵķ��ռ�����first����
						{
							(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
							(firstMap.find(nonTerStr)->second).erase("?");
						}
					}
					else//����ղ��ڸ÷��ռ�����first�����У������������ˣ�ֱ��break
					{
						//firstMap.find(nonTerStr)->second)�Ǹò���ʽ���ķ��ռ�����Ӧ��first��
						(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
						break;//����ʽ�Ҳ��������ķ����ŵ�first���в����գ����Բ��������Ҳ��������
					}

				}
				else//�ò���ʽ�Ҳ������ķ��������ռ���
				{
					(firstMap.find(nonTerStr)->second).insert(*producRightIter);
					break;//����ʽ�Ҳ��������ķ��������ռ��������Բ��������Ҳ��������
				}
			}
		}
	}
}


void productionList::establishFollowMap()
{
	for (int i = 0; i < 10; i++)//Ϊ��֤�պϣ���������
	{
		set<string>::iterator NonTermimalIter;//�������ռ������ϵĵ�����
		for (NonTermimalIter = nonTerminalSet.begin(); NonTermimalIter != nonTerminalSet.end(); NonTermimalIter++)
		{
			findFollowSet(*NonTermimalIter);
		}
	}
}


void productionList::findFollowSet(string nonTerStr)
{
	for (list<production>::iterator producIter = producList.begin(); producIter != producList.end(); producIter++)//����ÿһ������ʽ
	{
		int count = countStrInAVec(nonTerStr, producIter->productionRight);//����ʽ�Ҳ��г��ֵ�
		if (count == 0)
		{
			continue;
		}
		else//����ʽ�Ҳ��ж�����������ķ��ռ���
		{
			/*****************************************************************************************************************************/
			for (int i = 0; i < count; i++)
			{
				list<string> strList;//��¼���ķ������Ҳ���ķ�����
				strList.insert(end(strList), findPosStrInAVec(i + 1, nonTerStr, (*producIter).productionRight), (*producIter).productionRight.end());
				if (strList.empty())
				{
					(followMap.find(nonTerStr)->second).insert((followMap.find((*producIter).productionLeft)->second).begin(), (followMap.find((*producIter).productionLeft)->second).end());
				}
				//ɨ����ɺ󣬽�����λ�ڸ÷��ռ����Ҳ�������ķ����ŵ�strList��first��������first�����뵽�÷��ռ�����follow���У�
				//ͬʱҲ������жϲ�ʵʩ�Ƿ���Ҫ������ʽ���ķ��ռ�����follow�����뵽�÷��ռ�����follow��������
				addItemToFollow(producIter->productionLeft, nonTerStr, strList);
			}
			/*****************************************************************************************************************************/
		}

	}


}

int productionList::countStrInAVec(string str, vector<string>& strVec)
{
	int count = 0;
	for (vector<string>::iterator strVecIter = strVec.begin(); strVecIter != strVec.end(); strVecIter++)
	{
		if (*strVecIter == str)
		{
			count++;
		}
	}
	return count;

}

vector<string>::iterator productionList::findPosStrInAVec(int seq, string str, vector<string>& strVec)
{
	int count = 0;
	for (vector<string>::iterator vecIter = strVec.begin(); vecIter != strVec.end(); vecIter++)
	{
		if (*vecIter == str)
		{
			count++;
		}
		if (count == seq)
		{
			return vecIter + 1;
		}
	}
	return strVec.end();
}


void productionList::addItemToFollow(string productionLeft, string nonTerStr, list<string>& strList)
{
	if (strList.empty())//�������ʽ�Ҳ�û�������ķ�����(���账��)�������ķ����������Ҳ�(��ǰ����ֱ�Ӵ���)
	{
		return;//���Ż���Ĵ����У��������Ӧ�ò�����֡���Ϊ֮ǰ�Ѿ�ͨ��count�ж��˸ò���ʽ�Ҳൽ����û��������ռ���
	}
	else//�÷��ռ������������ʽ���Ҳ��Ҳ������ұߣ���Ҫ����
	{
		set<string> strSet;
		strSet = getStringFirstSet(strList);//ͨ�����Һ������ҵ�����ַ�����first��
		if (strSet.find("?") != strSet.end())//����ַ����ĵ�first�����п�
		{
			//�Ѳ���ʽ���ķ��ռ�����follow���ӽ�����ʽ�Ҳ��������ռ�����follow����
			(followMap.find(nonTerStr)->second).insert((followMap.find(productionLeft)->second).begin(), (followMap.find(productionLeft)->second).end());
			strSet.erase("?");
			//������ַ�����first�����뵽ǰ���Ǹ����ռ�����follow����
			(followMap.find(nonTerStr)->second).insert(strSet.begin(), strSet.end());
		}
		else//����ַ����ĵ�first����û�п�
		{
			//������ַ�����first�����뵽ǰ���Ǹ����ռ�����follow����
			(followMap.find(nonTerStr)->second).insert(strSet.begin(), strSet.end());
		}
	}
}

set<string> productionList::getStringFirstSet(list<string> strList)
{
	set<string> resultSet;
	list<string> ::iterator listIter;
	for (listIter = strList.begin(); listIter != strList.end(); listIter++)//����stringList
	{
		if (nonTerminalSet.find(*listIter) != nonTerminalSet.end())//��һ���ķ������Ƿ��ռ���
		{
			if (firstMap.find(*listIter)->second.find("?") != firstMap.find(*listIter)->second.end())//�˷��ռ�����first�����п�
			{
				resultSet.insert(firstMap.find(*listIter)->second.begin(), firstMap.find(*listIter)->second.end());
				if (*listIter == strList.back())//�����һ�����ţ��Ͳ���ɾȥ��
				{

				}
				else//�������һ�����ţ���Ҫɾȥ��
				{
					resultSet.erase("?");
				}
			}
			else//�˷��ռ�����first�����޿�,������������
			{
				resultSet.insert(firstMap.find(*listIter)->second.begin(), firstMap.find(*listIter)->second.end());
				break;//����forѭ��
			}
		}
		else//��һ���ķ�������һ���ռ�������������������
		{
			resultSet.insert(*listIter);
			break;//����forѭ��
		}
	}

	return resultSet;
}

void productionList::establishPredictMap()
{
	list<production>::iterator procListIter;
	vector<string>::iterator strIt;
	for (procListIter = producList.begin(); procListIter != producList.end(); procListIter++)//����ÿһ������ʽ
	{
		list<string> strList;
		set<string> strSet;
		for (strIt = (*procListIter).productionRight.begin(); strIt != (*procListIter).productionRight.end(); strIt++)
		{
			strList.push_back(*strIt);
		}
		strSet = getStringFirstSet(strList);
		if (strSet.find("?") != strSet.end())//���ķ����Ŵ���first�����п�,predict(�ò���ʽ)=first(���ķ����Ŵ�)-{��}+follow(�ò���ʽ���ķ��ռ���)
		{
			strSet.erase("?");
			predictMap.find(*procListIter)->second.insert(strSet.begin(), strSet.end());
			predictMap.find(*procListIter)->second.insert(followMap.find(procListIter->productionLeft)->second.begin(), followMap.find(procListIter->productionLeft)->second.end());
		}
		else//���ķ����Ŵ���first����û�пգ�predict(�ò���ʽ)=first(���ķ����Ŵ�)
		{
			predictMap.find(*procListIter)->second.insert(strSet.begin(), strSet.end());
		}
	}
}


/*******************************************************************************************/
/*******************************************************************************************/
void productionList::storeFirstMap()
{

	fstream outputFile;
	outputFile.open("D:/Documents/bianyi/experiment/LL1/CompilerData/FirstSet.txt");
	if (!outputFile)
	{
		cout << "FirstSet�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "First Map: " << endl << endl;
	}

	for (map<string, set<string>>::iterator firstMapIter = firstMap.begin(); firstMapIter != firstMap.end(); firstMapIter++)
	{
		outputFile << firstMapIter->first << ": { ";
		for (set<string>::iterator setIter = firstMapIter->second.begin(); setIter != firstMapIter->second.end(); setIter++)
		{
			outputFile << *setIter << " ";
		}
		outputFile << " }" << endl;
	}
	outputFile << endl << endl;
	outputFile.close();

}


void productionList::storeFollowMap()
{
	fstream outputFile;
	outputFile.open("D:/Documents/bianyi/experiment/LL1/CompilerData/FollowSet.txt");
	if (!outputFile)
	{
		cout << "FollowSet�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "Follow Map: " << endl << endl;
	}

	for (map<string, set<string>>::iterator followMapIter = followMap.begin(); followMapIter != followMap.end(); followMapIter++)
	{
		outputFile << followMapIter->first << ": { ";
		for (set<string>::iterator setIter = followMapIter->second.begin(); setIter != followMapIter->second.end(); setIter++)
		{
			outputFile << *setIter << " ";
		}
		outputFile << " }" << endl;
	}

	outputFile << endl << endl;
	outputFile.close();
}


void productionList::storePredictMap()
{
	fstream outputFile;
	outputFile.open("D:/Documents/bianyi/experiment/LL1/CompilerData/PredictSet.txt");
	if (!outputFile)
	{
		cout << "PredictSet�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "Predict Map: " << endl << endl;
	}

	for (map<production, set<string>>::iterator predictMapIter = predictMap.begin(); predictMapIter != predictMap.end(); predictMapIter++)
	{
		outputFile << predictMapIter->first.sequenceNum << ")    ";
		outputFile << " { ";
		for (set<string>::iterator setIter = predictMapIter->second.begin(); setIter != predictMapIter->second.end(); setIter++)
		{
			outputFile << *setIter << " ";
		}
		outputFile << " }" << endl;
	}
	outputFile << endl << endl;

	outputFile.close();
}


void  productionList::storeNonTerminalSet()
{

	fstream outputFile;
	outputFile.open("D:/Documents/bianyi/experiment/LL1/CompilerData/NonTerminalSet.txt");
	if (!outputFile)
	{
		cout << "NonTerminalSet�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "NonTerminal Set: " << endl << endl;
	}

	for (set<string>::iterator iter = nonTerminalSet.begin(); iter != nonTerminalSet.end(); iter++)
	{
		outputFile << *iter << endl;
	}

	outputFile << endl << endl;
	outputFile.close();

}


void  productionList::storeTerminalSet()
{
	fstream outputFile;
	outputFile.open("D:/Documents/bianyi/experiment/LL1/CompilerData/TerminalSet.txt");
	if (!outputFile)
	{
		cout << "TerminalSet�ļ���ʧ�ܣ�" << endl;
		return;
	}
	else
	{
		outputFile << "Terminal Set: " << endl << endl;
	}

	for (set<string>::iterator iter = TerminalSet.begin(); iter != TerminalSet.end(); iter++)
	{
		outputFile << *iter << endl;
	}

	outputFile << endl << endl;
	outputFile.close();
}

