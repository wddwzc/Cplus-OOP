#include "GA.h"

GAFunc::GAFunc() : limitWeight(80), limitVolume(75), popSize(200), 
					geneNum(32), probCross(0.6), probMutate(0.05)
{
	initPop();
}

GAFunc::GAFunc(int limitW, int limitV, int popS, int geneN, double probC,
				double probM, int *WE, int *VO, int *VA)
	: limitWeight(limitW), limitVolume(limitV), popSize(popS), geneNum(geneN), probCross(probC),
		probMutate(probM), weightInfo(WE), volumeInfo(VO), valueInfo(VA)
{
	initPop();
}

GAFunc::~GAFunc() {

}

void GAFunc::iterateRun(int runNum) {
	int maxFitness = 0, count = 0;
	for (int i = 1; i <= runNum; ++i) {
		selectPop();
		crossPop();
		mutationPop();
		updatePop();
		std::cout << "Generation: " << i << std::endl;
		// 当最大适应度连续八十代不变时，认为已经收敛到最优解
		if (maxFitness == population[0].fitness) {
			++count;
			if (count == 80) {
				break;
			}
		}
		else {
			count = 0;
		}
		maxFitness = population[0].fitness;
	}
	selectPop();
}

void GAFunc::printPopulation() {
	int val = 0;
	for (auto i : population) {
		val = (val < i.fitness ? i.fitness : val);
		for (auto j : i.gene)
			std::cout << j;
		std::cout << " " << i.fitness << " " << i.volume << " " << i.weight << std::endl;

	}
	std::cout << " Optimal fitness: " << val << std::endl;
	std::cout << " Optimal entity:  ";
	for (auto i : population[0].gene) {
		std::cout << i;
	}
	std::cout << std::endl;
}

void GAFunc::initPop() {
	std::cout << "Initialize" << std::endl;
	Entity tempEntity;
	for (int i = 0; i < popSize; ++i) {
		while (1) {
			tempEntity.gene.clear();
			for (int j = 0; j < geneNum; ++j) {
				int r = randNum(1000);
				if (r <= 300) {
					tempEntity.gene.push_back(1);
				}
				else {
					tempEntity.gene.push_back(0);
				}
			}
			setTotalWeight(tempEntity);
			setTotalVolume(tempEntity);
			// 初始化种群时，过滤掉不符合要求的个体
			if (tempEntity.volume <= limitVolume && tempEntity.weight <= limitWeight) {
				setFitness(tempEntity);
				population.push_back(tempEntity);
				break;
			}
		}
	}
}

void GAFunc::selectPop() {
	int totalFitness = getTotalFit(population);
	double rate = 0.0, currentRate = 0.0;
	std::vector<Entity> population_next;
	sortPop();
	// 保留适应度前四的个体，不进行杂交和变异
	for (int i = 0; i < 4; ++i) {
		population_next.push_back(population[i]);
	}
	// 其它个体用轮盘赌进行选择
	for (int i = 4; i < popSize; ++i) {
		rate = randNum(1000) / 1000.0;
		for (auto entity : population) {
			currentRate += (double) entity.fitness / totalFitness;
			if (rate < currentRate) {
				population_next.push_back(entity);
				currentRate = 0.0;
				break;
			}
		}
	}
	population.swap(population_next);
}

void GAFunc::crossPop() {
	double rateCross = 0.0;
	int crossSite = 0;
	for (int i = 4; i < popSize; i += 2) {
		rateCross = randNum(1000) / 1000.0;
		crossSite = randNum(geneNum);
		Entity &crossEntity1 = population[i];
		Entity &crossEntity2 = population[i + 1];
		if (rateCross < probCross) {
			for (int i = 0; i < geneNum; ++i) {
				crossEntity1.gene[i] = (i < crossSite ? crossEntity1.gene[i] : crossEntity2.gene[i]);
				crossEntity2.gene[i] = (i < crossSite ? crossEntity2.gene[i] : crossEntity1.gene[i]);
			}
		}
	}
}

void GAFunc::mutationPop() {
	double rateMutation = 0.0;
	for (int i = 4; i < popSize; ++i) {
		Entity &mutationEntity = population[i];
		for (auto &gene : mutationEntity.gene) {
			rateMutation = randNum(1000) / 1000.0;
			if (rateMutation < probMutate) {
				gene = (gene ? 0 : 1);
			}
		}
	}
}

void GAFunc::updatePop() {
	for (auto &entity : population) {
		setTotalVolume(entity);
		setTotalWeight(entity);
		setFitness(entity);
	}
}

void GAFunc::sortPop() {
	Entity tempEntity;
	for (int i = 1; i < popSize - 1; ++i) {
		for (int j = 0; j < popSize - i; ++j) {
			if (population[j].fitness < population[j + 1].fitness) {
				tempEntity = population[j];
				population[j] = population[j + 1];
				population[j + 1] = tempEntity;
			}
		}
	}
}

int GAFunc::randNum(int upper, int lower) {
	return (lower + rand() % (upper - lower));
}

void GAFunc::setTotalWeight(Entity &entity) {
	int totalweight = 0;
	for (int i = 0; i < geneNum; ++i) {
		totalweight += entity.gene[i] * weightInfo[i];
	}
	entity.weight = totalweight;
}

void GAFunc::setTotalVolume(Entity &entity) {
	int totalvolume = 0;
	for (int i = 0; i < geneNum; ++i) {
		totalvolume += entity.gene[i] * volumeInfo[i];
	}
	entity.volume = totalvolume;
}

void GAFunc::setFitness(Entity &entity) {
	int fitness = 0;
	if (entity.volume <= limitVolume && entity.weight <= limitWeight) {
		for (int i = 0; i < geneNum; ++i) {
			fitness += entity.gene[i] * valueInfo[i];
		}
	}
	entity.fitness = fitness;
}

int GAFunc::getTotalFit(std::vector<Entity> &pop) {
	int totalfit = 0;
	for (auto entity : pop) {
		totalfit += entity.fitness;
	}
	return totalfit;
}
