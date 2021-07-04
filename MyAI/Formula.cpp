#include "Formula.h"

Formula::Formula(std::vector<step> steps)
{
	this->steps = steps;
	this->score = 0;
}

float Formula::getResult(float value)
{
	int i = 0;
	float res = value;
	for (i = 0; i < this->steps.size(); i++)
	{
		step s = this->steps[i];
		if (s.type == ADD)
		{
			res += s.value;
		}
		else if (s.type == MULTI)
		{
			res *= s.value;
		}
		else
		{
			throw std::exception("err type");
		}
	}

	return res;
}

std::vector<step> Formula::getSteps()
{
	return this->steps;
}

std::string Formula::toString()
{
	int i = 0;
	std::string res = "";

	res += "value ";
	for (i = 0; i < this->steps.size(); i++)
	{
		step s = this->steps[i];
		if (s.type == ADD)
		{
			res += "+ ";
		}
		else if (s.type == MULTI)
		{
			res += "x ";
		}
		else
		{
			throw std::exception("err type");
		}
		res += std::to_string(s.value) + " ";
	}

	return res;
}

void Formula::setSteps(std::vector<step> steps)
{
	this->steps = steps;
}

void Formula::merageSteps()
{
	// if the same steps in a row we cam merge them
	int i = 0;
	while (i + 1 < this->steps.size())
	{
		step s = this->steps[i];
		step nextS = this->steps[i + 1];
		if (s.type == nextS.type)
		{
			int type = s.type;
			step mergedStep;
			mergedStep.type = type;
			if (type == ADD)
			{
				mergedStep.value = s.value + nextS.value;
			}
			else if (type == MULTI)
			{
				mergedStep.value = s.value * nextS.value;
			}

			// delete next
			this->steps.erase(this->steps.begin() + i + 1);
		}
		else
		{
			i++;
		}
	}
}

void Formula::prittier()
{
	int i = 0;
	while (i < this->steps.size())
	{
		step s = this->steps[i];
		if (s.type == ADD && s.value == 0 ||
			s.type == MULTI && s.value == 1)
		{
			this->steps.erase(this->steps.begin() + i);
		}
		else
		{
			i++;
		}
	}

	this->merageSteps();
}