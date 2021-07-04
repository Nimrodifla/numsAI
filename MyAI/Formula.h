#pragma once
#include <vector>
#include <math.h>
#include <string>

enum types { ADD, MULTI};

struct step
{
	float value;
	int type;
} typedef step;

class Formula
{
private:
	std::vector<step> steps;

	void merageSteps();

public:
	int score;

	Formula(std::vector<step> steps);
	~Formula() = default;

	float getResult(float value);
	std::vector<step> getSteps();
	std::string toString();
	void setSteps(std::vector<step> steps);
	void prittier();
};