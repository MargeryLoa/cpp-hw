#pragma once

typedef struct
{
	double Re, Im;
} t_compl;

class Equation
{
	double mA, mB, mC;

public:

	Equation();
	Equation(double A, double B, double C);

	Equation& operator=(const Equation& E);

	void ParserEquation(std::string Str);
	
	void PrintEq();
	int CheckBadEq();
	void CountEq(t_compl* X1, t_compl* X2);
};
