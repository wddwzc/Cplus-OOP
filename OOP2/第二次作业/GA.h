#ifndef GA_H
#define GA_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

// 个体信息结构
class Entity {
public:
	std::vector<int> gene;	// 基因
	int weight;	// 个体重量
	int volume;	// 个体体积
	int fitness;	// 个体适应度
};

class GAFunc
{
public:
	GAFunc();
	// 重量限制	体积限制	种群大小	个体基因数量	交叉概率	变异概率	重量数据	体积数据	价值数据
	GAFunc(int limitW, int limitV, int popS, int geneN, double probC, double probM, int *WE, int *VO, int *VA);
	~GAFunc();
	// 进行遗传迭代
	void iterateRun(int runNum);
	// 输出种群信息
	void printPopulation();

private:
	// 生成初始种群
	void initPop();
	// 染色体选择
	void selectPop();
	// 染色体交叉
	void crossPop();
	// 染色体突变
	void mutationPop();
	// 更新种群信息
	void updatePop();
	// 根据适应度对个体进行降序排序
	void sortPop();

	// 根据上下限生成随机整型数
	int randNum(int upper, int lower = 0);
	// 计算并设置个体总重量
	void setTotalWeight(Entity &entity);
	// 计算并设置个体总体积
	void setTotalVolume(Entity &entity);
	// 计算并设置个体适应度（总价值）
	void setFitness(Entity &entity);
	// 获取种群总适应度
	int getTotalFit(std::vector<Entity> &pop);

	std::vector<Entity> population;	// 种群信息
	int *weightInfo;	// 物品重量信息
	int *volumeInfo;	// 物品体积信息
	int *valueInfo;		// 物品价值信息
	const int limitWeight;	// 容器重量上限
	const int limitVolume;	// 容器容积上限
	const int popSize;	// 种群大小
	const int geneNum;	// 染色体上基因数量
	const double probCross;	// 交叉概率
	const double probMutate;	// 突变概率

};

#endif