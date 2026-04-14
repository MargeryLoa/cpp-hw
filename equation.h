#pragma once
class Equation
{
public:
	double mA, mB, mC;

	Equation(void);
	Equation(double A, double B, double C);

	Equation& operator=(const Equation& E);

	void ParserEquation(std::string Str);
};