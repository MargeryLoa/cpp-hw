#include <iostream>
#include <fstream>
#include <sstream>

#include "equation_list.h"
#include "student.h"


EquatList::EquatList(Equation E)
{
	mEquation = E;
	mNext = nullptr;
}

void EquatList::Add(Equation E)
{
	EquatList* C = this;
	
	if (C != nullptr && C->mNext == nullptr && C->mEquation.CheckBadEq())
		C->mEquation = E;
	else
	{
		while (C->mNext != nullptr)
			C = C->mNext;
		C->mNext = new EquatList(E);
	}
}

void EquatList::Print()
{
	EquatList* C = this;

	if (C == nullptr || (C != nullptr && C->mNext == nullptr && C->mEquation.CheckBadEq()))
	{
		std::cout << "List of Equations is empty\n";
		return;
	}

	while (C->mNext != nullptr)
	{
		C->mEquation.PrintEq();
		C = C->mNext;
	}
	C->mEquation.PrintEq();
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

int EquatList::SizeOfList()
{
	EquatList* C = this;
	int Num = 0;

	if (C != nullptr && C->mNext == nullptr && C->mEquation.CheckBadEq())
		return 0;
	else
		while (C->mNext != nullptr)
			C = C->mNext, Num++;
	return Num+1;
}

void EquatList::Remove()
{
	if (mEquation.CheckBadEq() && mNext == nullptr)
		return;

	if (mNext != nullptr)
	{
		mEquation = mNext->mEquation;
		EquatList* toDelete = mNext;
		mNext = mNext->mNext;
		free(toDelete);
	}
	else
	{
		mEquation = Equation();
		mNext = nullptr;
	}

}


EquatList::~EquatList()
{
	while (mNext != nullptr) {
		Remove();
	}
}
