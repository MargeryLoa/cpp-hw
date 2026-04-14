#include <iostream>
#include <cstdio>
#include <sstream>
#include <locale>

#include "equation.h"

Equation::Equation()
{
	mA = mB = mC = 0;
}

Equation::Equation(double A, double B, double C)
{
	mA = A;
	mB = B;
	mC = C;
}

Equation& Equation::operator=(const Equation & E)
{
	mA = E.mA;
	mB = E.mB;
	mC = E.mC;

	return *this;
}

void Equation::ParserEquation(std::string Str)
{
	//std::sscanf(Str.c_str(), "%lf %lf %lf", &mA, &mB, &mC);
	std::istringstream iss(Str);
	iss.imbue(std::locale("C")); // дес€тичный разделитель Ч точка
	iss >> mA >> mB >> mC;
}