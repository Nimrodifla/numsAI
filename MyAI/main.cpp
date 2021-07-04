#include <string>
#include <iostream>
#include "Formula.h"
#include <chrono>
using namespace std::chrono;

#define EXIT_OUT -7
#define BASE_VAL 1000
#define FORMS_TO_KEEP 5
#define MAX_ALLOWED_DELTA 0

struct inOut
{
	float in;
	float out;
} typedef inOut;

int generateFormula(float input, float output, std::vector<Formula*>& forms);
Formula* getBestFormula(std::vector<Formula*>& forms);
float definedValue(float x);
void vote(inOut io, std::vector<Formula*>& forms);
void resetScores(std::vector<Formula*>& forms);
float calcDelta(float a, float b);
int randomNum();

// global
int rnd_num = 0;

int main()
{
	// init random
	rnd_num = randomNum();

	// GET TRAIN-DATA
	std::vector<Formula*> formulas;
	std::vector<inOut> trainData;
	// init formulas
	std::vector<step> s;
	step st;
	st.type = ADD;
	st.value = 0;
	s.push_back(st);
	Formula* f = new Formula(s);
	formulas.push_back(f);


	int i = 0;
	float in = 0, out = 0;
	bool flag = true;
	while (flag)
	{
		std::cout << "Input: ";
		std::cin >> in;
		std::cout << "Output (" + std::to_string(EXIT_OUT) + " to exit): ";
		std::cin >> out;

		if (out == EXIT_OUT)
		{
			flag = false;
		}
		else
		{
			//generateFormula(in, out, formulas);
			inOut data;
			data.in = in;
			data.out = out;
			trainData.push_back(data);
		}
	}

	auto start = high_resolution_clock::now();

	// TRAIN UNTILL U R GOOD
	flag = true;
	while (flag)
	{
		// improve
		float avgDelta = 0;
		bool allGood = true;
		Formula* f = getBestFormula(formulas);
		float d = f->getResult(trainData[0].in) - trainData[0].out;
		bool dIsEquel = true;
		for (i = 0; i < trainData.size(); i++)
		{
			generateFormula(trainData[i].in, trainData[i].out, formulas);
			float temp = f->getResult(trainData[i].in) - trainData[i].out;
			avgDelta += temp;
			if (definedValue(temp) > MAX_ALLOWED_DELTA)
				allGood = false;
			if (d != temp)
			{
				dIsEquel = false;
			}
		}

		avgDelta /= trainData.size();

		if (dIsEquel)
		{
			std::vector<step> temp = f->getSteps();
			step s;
			s.type = ADD;
			s.value = -d;
			temp.push_back(s);
			f->setSteps(temp);
			f->prittier();
		}
		

		system("cls");
		std::cout << "Avg Delta: " << avgDelta << "\n";
		std::cout << "Best Formula: " << f->toString() << "\n";

		if (allGood)
		{
			flag = false;
			break;
		}

		resetScores(formulas);
		// vote
		for (i = 0; i < trainData.size(); i++)
		{
			vote(trainData[i], formulas);
		}
		// delete
		while (formulas.size() > FORMS_TO_KEEP)
		{
			// delete lowest
			int min = formulas[0]->score;
			int secMinIndex = 0;
			for (int j = 0; j < formulas.size(); j++)
			{
				if (formulas[j]->score < min)
				{
					secMinIndex = j;
					min = formulas[j]->score;
				}
			}
			delete formulas[secMinIndex];
			formulas.erase(formulas.begin() + secMinIndex); // delete lowest
		}
		
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "time: " << duration.count() << "\n\n";

	// WORK!
	Formula* formula = getBestFormula(formulas);

	flag = true;
	while (flag)
	{
		std::cout << "Input (" + std::to_string(EXIT_OUT) + " to exit): ";
		std::cin >> in;

		if (in == EXIT_OUT)
		{
			flag = false;
		}
		else
		{
			std::cout << formula->getResult(in) << "\n";
		}
	}


	return 0;
}

int generateFormula(float input, float output, std::vector<Formula*>& forms)
{
	// check what formula gives the closest result
	float minDelta = calcDelta(forms[0]->getResult(input), output);
	int minIndex = 0;
	int i = 0;
	for (i = 0; i < forms.size(); i++)
	{
		if (calcDelta(forms[i]->getResult(input), output) < minDelta)
		{
			minDelta = definedValue(forms[i]->getResult(input) - output);
			minIndex = i;
		}
	}

		
		
	Formula* bestYet = new Formula(*(forms[minIndex]));

	bestYet->score = 0;
	// add step OR change one OR delete one
	int choice = 0;
	// go over all the steps
	std::vector<step> steps = bestYet->getSteps();
	i = 0;
	while (i < steps.size())
	{
		choice = randomNum() % 3;
		if (choice == 0)
		{
			// change
			choice = randomNum() % 3;
			int max = BASE_VAL;
			if (choice != 0)
			{
				max /= 10;
				if (choice == 2)
				{
					max /= 10;
				}
			}
			float val = randomNum() % max; // change if u want
			choice = randomNum() % 2;
			if (choice == 1)
				steps[i].value += val;
			else
				steps[i].value -= val;

			i++;
		}
		else if (choice == 1)
		{
			// delete
			steps.erase(steps.begin() + i);
		}
		// choice == 2 -> do nothing to this step
	}
	choice = randomNum() % 2;
	// add step?
	if (choice == 1 || steps.size() == 0)
	{
		// add
		step s;
		s.type = randomNum() % 2; // only 2 types
		choice = randomNum() % 3;
		int max = BASE_VAL;
		if (choice != 0)
		{
			max /= 10;
			if (choice == 2)
			{
				max /= 10;
			}
		}
		s.value = randomNum() % max; // change if you want
		steps.push_back(s);
	}
	bestYet->setSteps(steps);

	bestYet->prittier();
	forms.push_back(bestYet);

	return 0;
}

Formula* getBestFormula(std::vector<Formula*>& forms)
{
	int max = 0;
	int maxIndex = 0;
	int i = 0;

	for (i = 0; i < forms.size(); i++)
	{
		if (forms[i]->score > max)
		{
			max = forms[i]->score;
			maxIndex = i;
		}
	}

	return forms[maxIndex];
}

float definedValue(float x)
{
	if (x < 0)
	{
		return x * -1;
	}

	return x;
}

void vote(inOut io, std::vector<Formula*>& forms)
{
	float input = io.in;
	float output = io.out;
	std::vector<Formula*> voteVec;
	std::vector<Formula*> tempVec;
	tempVec = forms;

	// orginize the formulas by the least delta to the most
	int i = 0;
	while (tempVec.size() > 0)
	{
		float minDelta = calcDelta(tempVec[0]->getResult(input), output);
		int minIndex = 0;
		for (i = 0; i < tempVec.size(); i++)
		{
			if (calcDelta(tempVec[i]->getResult(input), output) < minDelta)
			{
				minDelta = definedValue(tempVec[i]->getResult(input) - output);
				minIndex = i;
			}
		}
		voteVec.push_back(tempVec[minIndex]);
		tempVec.erase(tempVec.begin() + minIndex);
	}

	// voting time!
	for (i = 0; i < voteVec.size(); i++)
	{
		voteVec[i]->score += (voteVec.size() - i - 1);
	}
}

void resetScores(std::vector<Formula*>& forms)
{
	int i = 0;
	for (i = 0; i < forms.size(); i++)
	{
		forms[i]->score = 0;
	}
}

float calcDelta(float a, float b)
{
	return definedValue(a - b);
}

int randomNum()
{
	std::srand(std::time(nullptr));

	rnd_num += 113 % (std::rand() + 1);
	if (rnd_num > 1000)
	{
		rnd_num = 7 % (std::rand() + 1);
	}
	return (std::rand() + rnd_num);
}