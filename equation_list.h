#include <iostream>

#include "equation.h"


#pragma once
class EquatList
{
public:
	Equation mEquation;
	EquatList* mNext = nullptr;

	EquatList() = default;
	EquatList(Equation E);

	void Add(Equation E);
	void Remove();
	
	void Print();
	void ReadFile(std::string FileName);
	int SizeOfList();

	~EquatList();
};
