#include "pascalchecker.h"

namespace grammcheck {

	// 默认初始化 关键字字典
	PascalChecker::PascalChecker() {
		keywords = { "begin", "end", "if", "else", "then", "end.", "end;" };
		endflag = false;
	}

	// 加载pascal源文件
	bool PascalChecker::LoadSrcFile(string srcfile) {
		ifstream inf(srcfile);
		string src;
		ClearData();	// 加载文件是清空当前数据
		// 文件输入流无效时提示并返回
		if (!inf) {
			cout << "The path of source file is valid!" << endl;
			return false;
		}
		// 按行读取pascal源文件
		while (!inf.eof()) {
			getline(inf, src);
			sourcecode.push_back(src);
		}
		// 文件为空时，输出提示并返回，无须分析
		if (!sourcecode.size()) {
			cout << "The file you opened is empty!" << endl;
			return false;
		}
		cout << "The analysis result is as follows:" << endl;
		return true;
	}

	// 分析pascal源文件关键字匹配
	void PascalChecker::SyntaxAnalyzer() {
		for (int i = 0; i < sourcecode.size(); ++i) {
			string codeline = sourcecode[i];
			// 将大写字母都转换为小写，因为pascal语言大小写不敏感
			for (auto &c : codeline) {
				c = tolower(c);
			}
			string word;
			istringstream istring(codeline);
			while (istring >> word) {
				// 在字典中查找当前word，确定是否为关键字
				if (keywords.find(word) != keywords.end()) {
					KeyInfo info(i+1, word);
					// 进行关键字匹配
					KeyMatch(info);	
				}
			}
		}
		// 匹配完成后，关键字栈中有残留，说明未匹配，将其存入错误信息
		if (keywords.size()) {
			for (auto &info : errorlist) {
				keystack.push_back(info);
			}
			errorlist.swap(keystack);

		}
	}

	// 输出源文件及分析结果
	void PascalChecker::PrintSrc() {
		cout << "**********************************" << endl;
		int i = 1;
		for (auto &src : sourcecode) {
			string errorflag = " ";
			// 如果本行存在错误，行前加*
			for (auto &error : errorlist) {
				if (i == error.index) {
					errorflag = "*";
				}
			}
			cout.width(3);
			cout << i++ << errorflag + src << endl;
		}
		cout << "************** error *************" << endl;
		// 输出错误信息
		for (auto key : errorlist) {
			cout << "line " << key.index << ": " + key.elem + " have no matching keywords" <<endl;
		}
	}

	// 清空当前缓存数据
	void PascalChecker::ClearData() {
		sourcecode.clear();
		keystack.clear();
		errorlist.clear();
		endflag = false;
	}

	// 关键字匹配
	void PascalChecker::KeyMatch(KeyInfo keyinfo) {
		auto iter_begin = keystack.begin(), iter_end = keystack.end();
		string lastkey, key = keyinfo.elem;
		KeyInfo lastinfo;
		if (iter_begin != iter_end) {		// 关键字栈非空时
			lastinfo = keystack[keystack.size() - 1];
			lastkey = lastinfo.elem;
			if (key == "begin" || key == "if") {
				if (lastkey == "if") {
					errorlist.push_back(lastinfo);
					keystack.pop_back();
					keystack.push_back(keyinfo);
				}
				else {
					keystack.push_back(keyinfo);
				}
			}
			else if (key == "then") {
				if (lastkey == "if") {
					keystack.push_back(keyinfo);
				}
				else {
					errorlist.push_back(keyinfo);
				}
			}
			else if (key == "else") {
				if (lastkey == "then") {
					keystack.pop_back();
					keystack.pop_back();
				}
				else {
					errorlist.push_back(keyinfo);
				}
			}
			else if (key == "end" || key == "end;") {
				if (lastkey == "begin") {
					keystack.pop_back();
				}
				else if (lastkey == "then") {
					keystack.pop_back();
					keystack.pop_back();
					KeyMatch(keyinfo);
				}
				else if (lastkey == "if") {
					keystack.pop_back();
					errorlist.push_back(lastinfo);
					KeyMatch(keyinfo);
				}
			}
		}
		else {		// 关键字栈为空时，说明在文件头
			if (key == "begin") {
				keystack.push_back(keyinfo);
			}
			else {
				errorlist.push_back(keyinfo);
			}
		}
	}

}
