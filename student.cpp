#include <iostream>
#include <cmath>
#include <cstdlib>

#include "student.h"

Student::Student()
{
	mName = "Aa", mSurname = "Bb";
	mGoodness = 1;
	mNameLen = 5;
}

Student::Student(std::string Name, std::string Surname, double G)
{
	mName = Name, mSurname = Surname;
	mGoodness = G;
	mNameLen = mName.length() + mSurname.length() + 1;
}

int Student::SolveEquation(t_compl* X1, t_compl* X2, Equation E)
{
	double IsSolved = (double)rand() / (double)RAND_MAX;

	if (mGoodness == 0 || mGoodness < IsSolved)
	{
		X1->Re = X2->Re = X1->Im = X2->Im = 0;
		return 0;
	}
	else 
	{
		E.CountEq(X1, X2);
		return 1;
	}

}

void Student::PrintStudent()
{
	std::cout << mSurname << " " << mName << " : " << mGoodness << std::endl;
}

void Student::SimplePrintStudent()
{
	std::cout << mSurname << " " << mName;
}


Student& Student::operator=(const Student& S)
{
	mName = S.mName;
	mSurname = S.mSurname;
	mGoodness = S.mGoodness;

	return *this;
}

bool Student::operator==(const Student& other) const
{
	return (mName == other.mName && mSurname == other.mSurname);
}

int Student::CheckZeroStudent()
{
	return (mName == "Aa");
}
