#include <iostream>
#include "GA.h"

using namespace std;

#define COUNT 200	// 最大迭代次数
#define GENE 32	// 基因（数据）数

int weight[GENE] = { 22, 15,  4,  5, 10, 19, 21, 20,  8, 13,  2,  3,  3, 17, 12,  5,
				     12,  4,  1, 21, 14, 23, 17, 15, 20, 22, 25,  0, 22, 15, 25, 13 };
int volume[GENE] = { 11, 22, 12, 21, 21, 13,  1, 10, 13,  8,  6, 25, 13, 27, 12, 23,
				     12, 24, 23, 11,  6, 24, 28, 10, 20, 13, 25, 23,  5, 26, 30, 15 };
int values[GENE] = {  8,  9, 15,  6, 16,  9,  1,  4, 14,  9,  3,  7, 12,  4, 15,  5,
					 18,  5, 15,  4,  6,  2, 12, 14, 11,  9, 13, 13, 14, 13, 19,  4 };

int main()
{
	srand((unsigned)time(NULL));
	GAFunc GA(80, 75, 200, GENE, 0.6, 0.05, weight, volume, values);
	GA.iterateRun(COUNT);
	GA.printPopulation();
	system("pause");
	return 0;
}
