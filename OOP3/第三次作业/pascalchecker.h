#ifndef PASCALCHECKER
#define PASCALCHECKER

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <set>
using namespace std;

namespace grammcheck {

	// 关键字相关数据 行号 键值
	class KeyInfo {
	public:
		KeyInfo() {};
		KeyInfo(int i, string ele) {
			index = i;
			elem = ele;
		};
		int index;	// 行号 标注所在行
		string elem;	// 键值 区分关键字
	};

	// 帕斯卡语言检查器
	class PascalChecker
	{
	public:
		PascalChecker();
		~PascalChecker() {};
		// 加载pascal源文件
		bool LoadSrcFile(string srcfile);
		// 分析pascal源文件关键字匹配
		void SyntaxAnalyzer();
		// 输出源文件及分析结果
		void PrintSrc();

	private:
		// 清空当前缓存数据
		void ClearData();
		// 关键字匹配
		void KeyMatch(KeyInfo key);

		// 关键字字典，保存了需要检查的所有关键字
		set<string> keywords;
		// 源文件缓存
		vector<string> sourcecode;
		// 关键字匹配用栈
		vector<KeyInfo> keystack;
		// 错误信息缓存
		vector<KeyInfo> errorlist;
		bool endflag;
	};
}

#endif
