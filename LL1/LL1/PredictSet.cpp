#include"PredictSet.h"

productionList::productionList()//默认构造函数
{

}

productionList::productionList(string dir)//参数为产生式文件所在的目录路径+文件名
{
	//填充产生式list和非终极符set
	readProducList(dir);

	//填充终极符set
	fillTerminalSet();

	//预先填充First集map，Follow集map，Predict集map的key值，便于下一步的构造
	InitialFirstFollowPredictMap();

	//构造每个非终极符的first集
	establishFirstMap();

	//构造每个非终极符的follow集
	establishFollowMap();

	//构造每个非终极符的predict集
	establishPredictMap();

	//实现从string到enum类型LexType的映射，方便构造LL1分析表
	//makeMapFromStringToLexType();

	//实现从string到enum类型NontmlType的映射，方便构造LL1分析表
	//makeMapFromStringToNontmlType();

}
/*****************************************************************************************/
//填充产生式list和非终极符set
/*****************************************************************************************/
void productionList::readProducList(string dir)
{
	ifstream infile;
	infile.open(dir);
	if (!infile)
	{
		cout << "ProducList文件打开失败！" << endl;
	}
	string line;//从文件中读取一行产生式
	string substr;//承载分割后的字符串
	production* ptrProduc;//指向一个产生式
	bool equalFlag = false;//标志是否已读到"::="这个符号
	int seqNum;//当前行号，也即产生式的编号


	while (getline(infile, line))
	{
		istringstream stream(line);//用于分割字符 它的作用是从string对象line中读取字符。istringstream类用于执行C++风格的串流的输入操作。
		ptrProduc = new production();//一行对应一个产生式

		stream >> substr;//每行的第一个元素必定是行号
		substr.pop_back();//可以删除最后一个元素.
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
				continue;//对等号不做任何处理
			}

			if (!equalFlag)//没读到::=
			{
				ptrProduc->productionLeft = substr;
				//cout << "Left:" << substr << endl;
				nonTerminalSet.insert(substr);//从产生式中提出非终极符，放入非终极符集
			}
			else
			{
				ptrProduc->productionRight.push_back(substr);//新的元素加到最后面，位置为当前最后一个元素的下一个元素
				//cout << "Right:" << substr << endl;
			}
		}


		producList.push_back(*ptrProduc);//将这个产生式链接入producList中
		equalFlag = false;//一行内容结束，将标志归位
		line.clear();
		substr.clear();
		//cout <<"This line is over!\n\n\n\n"<< endl;
	}
	infile.close();
	
}
/*****************************************************************************************/
//填充终极符集，便于之后的人工构造String到LexType的映射
/*****************************************************************************************/
void productionList::fillTerminalSet()
{
	list<production>::iterator iter;
	vector<string>::iterator it;
	for (iter = producList.begin(); iter != producList.end(); iter++)
	{
		for (it = (*iter).productionRight.begin(); it != (*iter).productionRight.end(); it++)
		{
			if (nonTerminalSet.find(*it) != nonTerminalSet.end())//该文法符号在非终极符集中
			{

			}
			else//该文法符号应该在终极符集中
			{
				TerminalSet.insert(*it);
			}
		}
	}
}

void productionList::InitialFirstFollowPredictMap()
{
	//一个空的string的set集
	set<string> nullstrset;

	//填充firstmap和followmap
	set<string>::iterator iter;
	for (iter = nonTerminalSet.begin(); iter != nonTerminalSet.end(); iter++)
	{
		firstMap.insert(pair<string, set<string>>(*iter, nullstrset));//如果值为1的key不存在，则把(1,"one")这个pair插入内
		followMap.insert(pair<string, set<string>>(*iter, nullstrset));//pair实质上是一个结构体，其主要的两个成员变量是first和second 
	}

	//对于开始符S，将"#"加入其follow集中
	followMap.find(producList.begin()->productionLeft)->second.insert("#");//->second即是成功与否的标志

	//填充predictmap
	list<production>::iterator produciter;
	for (produciter = producList.begin(); produciter != producList.end(); produciter++)
	{
		//map的key值是自定义类型，不能加进去的原因是没有重载比较运算符，没法比较，自然没法用insert函数--------------------已解决
		predictMap.insert(pair<production, set<string >>((*produciter), nullstrset));
	}

}

void productionList::establishFirstMap()
{
	for (int i = 0; i < 10; i++)//为保证闭合，多做几次
	{
		set<string>::iterator NonTermimalIter;//遍历非终极符集合的迭代器
		for (NonTermimalIter = nonTerminalSet.begin(); NonTermimalIter != nonTerminalSet.end(); NonTermimalIter++)
		{
			findFirstSet(*NonTermimalIter);
		}
	}


}


void productionList::findFirstSet(string nonTerStr)
{
	list<production>::iterator producIter;//遍历产生式链表的迭代器
	for (producIter = producList.begin(); producIter != producList.end(); producIter++)
	{
		if ((*producIter).productionLeft == nonTerStr)//找到符合条件的产生式
		{
			vector<string>::iterator producRightIter;//遍历该产生式右侧的文法符号vector的迭代器
			for (producRightIter = (*producIter).productionRight.begin(); producRightIter != (*producIter).productionRight.end(); producRightIter++)//遍历该产生式的右侧
			{
				if (nonTerminalSet.find(*producRightIter) != nonTerminalSet.end())//该文法符号是非终极符
				{
					/**<-------------(firstMap.find(*producRightIter)->second)是该产生式右侧的一个非终极符对应的firstSet------------------------>**/
					if ((firstMap.find(*producRightIter)->second).find("?") != (firstMap.find(*producRightIter)->second).end())//如果空在该非终极符的first集合中,还需要继续向右找，所以不用break
					{
						if (*producRightIter == (*producIter).productionRight.back())//若该文法符号是产生式最右侧的非终极符，且该终极符还能推出来空，那么这个产生式的左侧的非终极符的first集中必定有空
						{
							(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
						}
						else//若该文法符号不是产生式最右侧的非终极符，把该文法符号的first集除了空之外的元素加入到左边的非终极符的first集中
						{
							(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
							(firstMap.find(nonTerStr)->second).erase("?");
						}
					}
					else//如果空不在该非终极符的first集合中，不用向右找了，直接break
					{
						//firstMap.find(nonTerStr)->second)是该产生式左侧的非终极符对应的first集
						(firstMap.find(nonTerStr)->second).insert((firstMap.find(*producRightIter)->second).begin(), (firstMap.find(*producRightIter)->second).end());
						break;//产生式右侧遇到的文法符号的first集中不含空，所以不需再向右侧继续找了
					}

				}
				else//该产生式右侧的这个文法符号是终极符
				{
					(firstMap.find(nonTerStr)->second).insert(*producRightIter);
					break;//产生式右侧遇到的文法符号是终极符，所以不需再向右侧继续找了
				}
			}
		}
	}
}


void productionList::establishFollowMap()
{
	for (int i = 0; i < 10; i++)//为保证闭合，多做几次
	{
		set<string>::iterator NonTermimalIter;//遍历非终极符集合的迭代器
		for (NonTermimalIter = nonTerminalSet.begin(); NonTermimalIter != nonTerminalSet.end(); NonTermimalIter++)
		{
			findFollowSet(*NonTermimalIter);
		}
	}
}


void productionList::findFollowSet(string nonTerStr)
{
	for (list<production>::iterator producIter = producList.begin(); producIter != producList.end(); producIter++)//遍历每一个产生式
	{
		int count = countStrInAVec(nonTerStr, producIter->productionRight);//产生式右部有出现的
		if (count == 0)
		{
			continue;
		}
		else//产生式右部有多个符合条件的非终极符
		{
			/*****************************************************************************************************************************/
			for (int i = 0; i < count; i++)
			{
				list<string> strList;//记录该文法符号右侧的文法符号
				strList.insert(end(strList), findPosStrInAVec(i + 1, nonTerStr, (*producIter).productionRight), (*producIter).productionRight.end());
				if (strList.empty())
				{
					(followMap.find(nonTerStr)->second).insert((followMap.find((*producIter).productionLeft)->second).begin(), (followMap.find((*producIter).productionLeft)->second).end());
				}
				//扫描完成后，将包含位于该非终极符右侧的所有文法符号的strList求first集，将该first集加入到该非终极符的follow集中，
				//同时也完成了判断并实施是否需要将产生式左侧的非终极符的follow集加入到该非终极符的follow集的任务
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
	if (strList.empty())//这个产生式右侧没有所求文法符号(不需处理)或所求文法符号在最右侧(在前面已直接处理)
	{
		return;//在优化后的代码中，这种情况应该不会出现。因为之前已经通过count判断了该产生式右侧到底有没有所求非终极符
	}
	else//该非终极符在这个产生式的右侧且不在最右边，需要处理
	{
		set<string> strSet;
		strSet = getStringFirstSet(strList);//通过查找函数，找到这个字符串的first集
		if (strSet.find("?") != strSet.end())//这个字符串的的first集中有空
		{
			//把产生式左侧的非终极符的follow集加进产生式右侧的这个非终极符的follow集中
			(followMap.find(nonTerStr)->second).insert((followMap.find(productionLeft)->second).begin(), (followMap.find(productionLeft)->second).end());
			strSet.erase("?");
			//将这个字符串的first集加入到前面那个非终极符的follow集中
			(followMap.find(nonTerStr)->second).insert(strSet.begin(), strSet.end());
		}
		else//这个字符串的的first集中没有空
		{
			//将这个字符串的first集加入到前面那个非终极符的follow集中
			(followMap.find(nonTerStr)->second).insert(strSet.begin(), strSet.end());
		}
	}
}

set<string> productionList::getStringFirstSet(list<string> strList)
{
	set<string> resultSet;
	list<string> ::iterator listIter;
	for (listIter = strList.begin(); listIter != strList.end(); listIter++)//遍历stringList
	{
		if (nonTerminalSet.find(*listIter) != nonTerminalSet.end())//这一个文法符号是非终极符
		{
			if (firstMap.find(*listIter)->second.find("?") != firstMap.find(*listIter)->second.end())//此非终极符的first集中有空
			{
				resultSet.insert(firstMap.find(*listIter)->second.begin(), firstMap.find(*listIter)->second.end());
				if (*listIter == strList.back())//是最后一个符号，就不用删去空
				{

				}
				else//不是最后一个符号，需要删去空
				{
					resultSet.erase("?");
				}
			}
			else//此非终极符的first集中无空,不用向右找了
			{
				resultSet.insert(firstMap.find(*listIter)->second.begin(), firstMap.find(*listIter)->second.end());
				break;//跳出for循环
			}
		}
		else//这一个文法符号是一个终极符，不用再向右找了
		{
			resultSet.insert(*listIter);
			break;//跳出for循环
		}
	}

	return resultSet;
}

void productionList::establishPredictMap()
{
	list<production>::iterator procListIter;
	vector<string>::iterator strIt;
	for (procListIter = producList.begin(); procListIter != producList.end(); procListIter++)//遍历每一个产生式
	{
		list<string> strList;
		set<string> strSet;
		for (strIt = (*procListIter).productionRight.begin(); strIt != (*procListIter).productionRight.end(); strIt++)
		{
			strList.push_back(*strIt);
		}
		strSet = getStringFirstSet(strList);
		if (strSet.find("?") != strSet.end())//该文法符号串的first集中有空,predict(该产生式)=first(该文法符号串)-{空}+follow(该产生式左侧的非终极符)
		{
			strSet.erase("?");
			predictMap.find(*procListIter)->second.insert(strSet.begin(), strSet.end());
			predictMap.find(*procListIter)->second.insert(followMap.find(procListIter->productionLeft)->second.begin(), followMap.find(procListIter->productionLeft)->second.end());
		}
		else//该文法符号串的first集中没有空，predict(该产生式)=first(该文法符号串)
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
		cout << "FirstSet文件打开失败！" << endl;
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
		cout << "FollowSet文件打开失败！" << endl;
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
		cout << "PredictSet文件打开失败！" << endl;
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
		cout << "NonTerminalSet文件打开失败！" << endl;
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
		cout << "TerminalSet文件打开失败！" << endl;
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

