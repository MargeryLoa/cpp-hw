#include <iostream>

#include "equation.h"

#pragma once

typedef struct
{
	double Re, Im;
} t_compl;

class Student
{
public:
	std::string mName, mSurname;
	double mGoodness;

	Student(void);
	Student(std::string Name, std::string Surname, double G);

	int SolveEquation(t_compl* X1, t_compl* X2, Equation E);
};