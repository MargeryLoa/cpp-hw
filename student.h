#include <iostream>

#include "equation.h"

#pragma once

class Student
{
	std::string mName, mSurname;
public:
	double mGoodness;
	int mNameLen;

	Student();
	Student(std::string Name, std::string Surname, double G);

	int SolveEquation(t_compl* X1, t_compl* X2, Equation E);
	
	void PrintStudent();
	void SimplePrintStudent();
	int CheckZeroStudent();

	Student& operator=(const Student& S);
	//Student& operator=(const Student& S);
	bool operator==(const Student& other) const;
};
