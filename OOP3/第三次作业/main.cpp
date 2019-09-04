#include "pascalchecker.h"
#include <iostream>
using namespace grammcheck;

int main()
{
	PascalChecker checker;
	string path("pascal_src0.pas");
	// 如果默认文件有效，则输出分析结果
	if (checker.LoadSrcFile(path)) {
		checker.SyntaxAnalyzer();
		checker.PrintSrc();
	}
	// 在循环中输入文件名，可继续分析其他文件
	while (1) {
		cout << endl << "Please enter the path of the pascal source file!(enter exit to quit)" << endl;
		cin >> path;
		if (path == "exit") {
			return 0;
		}
		if (checker.LoadSrcFile(path)) {
			checker.SyntaxAnalyzer();
			checker.PrintSrc();
		}
	}
	return 0;
}
