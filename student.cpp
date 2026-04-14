#include <iostream>
#include <cmath>
#include <cstdlib>

#include "student.h"

Student::Student(void)
{
	mName = "Aa", mSurname = "Bb";
	mGoodness = 1;
}

Student::Student(std::string Name, std::string Surname, double G)
{
	mName = Name, mSurname = Surname;
	mGoodness = G;
}

int Student::SolveEquation(t_compl* X1, t_compl* X2, Equation E)
{
	double IsSolved = (double)rand() / (double)RAND_MAX, D;

	if (mGoodness == 0 || mGoodness < IsSolved)
	{
		X1->Re = X2->Re = X1->Im = X2->Im = 0;
		return 0;
	}
	else 
	{
		D = E.mB * E.mB - 4.0 * E.mA * E.mC;

		if (D > 0.0)
		{
			X1->Re = (-E.mB + sqrt(D)) / (2.0 * E.mA);
			X2->Re = (-E.mB - sqrt(D)) / (2.0 * E.mA);
			X1->Im = X2->Im = 0;
		}
		else if (D == 0)
		{
			X1->Re = X2->Re = -E.mB / (2.0 * E.mA);
			X1->Im = X2->Im = 0;
		}
		else if (D < 0)
		{
			X1->Re = X2->Re = -E.mB / (2.0 * E.mA);
			X1->Im = sqrt(-D) / (2.0 * E.mA);
			X2->Im = -sqrt(-D) / (2.0 * E.mA);
		}
		return 1;
	}

}