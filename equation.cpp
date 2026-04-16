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

int Equation::CheckBadEq()
{
	return (mA == 0);
}

void Equation::PrintEq()
{
	std::cout << mA << " * x ^ 2 + (" << mB << ") * x + (" << mC << ")\n";
}

void Equation::CountEq(t_compl* X1, t_compl* X2)
{
	double D = mB * mB - 4.0 * mA * mC;

	if (D > 0.0)
	{
		X1->Re = (-mB + sqrt(D)) / (2.0 * mA);
		X2->Re = (-mB - sqrt(D)) / (2.0 * mA);
		X1->Im = X2->Im = 0;
	}
	else if (D == 0)
	{
		X1->Re = X2->Re = -mB / (2.0 * mA);
		X1->Im = X2->Im = 0;
	}
	else if (D < 0)
	{
		X1->Re = X2->Re = -mB / (2.0 * mA);
		X1->Im = sqrt(-D) / (2.0 * mA);
		X2->Im = -sqrt(-D) / (2.0 * mA);
	}

}
