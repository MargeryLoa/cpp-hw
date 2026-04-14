#include <iostream>
#include "equation.h"

#pragma once
class EquatList
{
public:
	Equation mEquation;
	EquatList* mNext;

	EquatList(void);
	EquatList(Equation E);

	void Add(Equation E);
	void Print(void);
	void ReadFile(std::string FileName);
	int SizeOfList(void);
};