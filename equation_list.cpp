#include <iostream>
#include <fstream>
#include <sstream>

#include "equation_list.h"

EquatList::EquatList(void)
{
	mNext = nullptr;
}

EquatList::EquatList(Equation E)
{
	mEquation = E;
	mNext = nullptr;
}

void EquatList::Add(Equation E)
{
	EquatList* C = this;
	
	if (C != nullptr && C->mNext == nullptr && C->mEquation.mA == 0)
		C->mEquation = E;
	else
	{
		while (C->mNext != nullptr)
			C = C->mNext;
		C->mNext = new EquatList(E);
	}
}

void EquatList::Print(void)
{
	EquatList* C = this;

	if (C != nullptr && C->mNext == nullptr && C->mEquation.mA == 0)
	{
		std::cout << "List of Equations is empty\n";
		return;
	}

	while (C->mNext != nullptr)
	{
		std::cout << C->mEquation.mA << " " << C->mEquation.mB << " " << C->mEquation.mC << std::endl;
		C = C->mNext;
	}
	std::cout << C->mEquation.mA << " " << C->mEquation.mB << " " << C->mEquation.mC << std::endl;

}

void EquatList::ReadFile(std::string FileName)
{
	std::fstream F;
	std::string Str;

	F.open(FileName, std::fstream::in);
	while (std::getline(F, Str))
	{
		Equation E;

		E.ParserEquation(Str);
		Add(E);
	}

}

int EquatList::SizeOfList(void)
{
	EquatList* C = this;
	int Num = 0;

	if (C != nullptr && C->mNext == nullptr && C->mEquation.mA == 0)
		return 0;
	else
		while (C->mNext != nullptr)
			C = C->mNext, Num++;
	return Num+1;
}
