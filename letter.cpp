#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "letter_queue.h"

Letter::Letter(void)
{
	mSize = mReport = 0;
}

Letter::Letter(Student S, int NoE)
{
	mStudent.mName = S.mName;
	mStudent.mSurname = S.mSurname;
	mStudent.mGoodness = S.mGoodness;
	mSize = NoE;
	mReport = 0;

}

void Letter::Print(void)
{
	std::cout << "Ученик: " << mStudent.mSurname << " " << mStudent.mName << " ";
	std::cout << mReport;
	std::cout << std::endl;
}

LetterQueue::LetterQueue(void)
{
	mNext = nullptr;
}

LetterQueue::LetterQueue(Letter L)
{
	mLetter.mReport = L.mReport;
	mLetter.mSize = L.mSize;
	mLetter.mStudent = L.mStudent;
	mNext = nullptr;
}

void LetterQueue::Push(Student S, EquatList* EL)
{
	LetterQueue* C = this;
	Letter *L = new Letter(S, EL->SizeOfList());
	
	L->mReport = 0;

	if (C != nullptr && C->mNext == nullptr && C->mLetter.mSize == 0)
	{
		C->mLetter.mReport = L->mReport;
		C->mLetter.mStudent = L->mStudent;
		C->mLetter.mSize = L->mSize;
	}
	else
	{
		while (C->mNext != nullptr)
			C = C->mNext;
		C->mNext = new LetterQueue(*L);
	}
}

void LetterQueue::Pop(void) 
{
	if (mLetter.mSize == 0 && mNext == nullptr) 
		return;
	
	if (mNext != nullptr) 
	{
		mLetter = mNext->mLetter;
		LetterQueue* toDelete = mNext;
		mNext = mNext->mNext;
		delete toDelete;
	}
	else 
	{
		mLetter = Letter();
		mNext = nullptr;
	}
}

void LetterQueue::PrintList(void)
{
	LetterQueue* C = this;

	if ((C != nullptr && C->mNext == nullptr && C->mLetter.mStudent.mName == "Aa") || (C == nullptr))
	{
		std::cout << "List of Students is empty\n";
		return;
	}

	while (C->mNext != nullptr)
	{
		std::cout << C->mLetter.mStudent.mSurname << " " << C->mLetter.mStudent.mName << " " << C->mLetter.mStudent.mGoodness << std::endl;
		C = C->mNext;
	}
	std::cout << C->mLetter.mStudent.mSurname << " " << C->mLetter.mStudent.mName << " " << C->mLetter.mStudent.mGoodness << std::endl;

}

int LetterQueue::FindMaxLen(void)
{
	LetterQueue* C = this;
	int Len = 0, MaxLen = 0;

	if ((C != nullptr && C->mNext == nullptr && C->mLetter.mStudent.mName == "Aa") || (C == nullptr))
	{
		std::cout << "List of Students is empty\n";
		return 0;
	}

	while (C->mNext != nullptr)
	{
		Len = C->mLetter.mStudent.mName.length() + C->mLetter.mStudent.mSurname.length() + 1;
		if (Len > MaxLen)
			MaxLen = Len;
		C = C->mNext;
	}
	Len = C->mLetter.mStudent.mName.length() + C->mLetter.mStudent.mSurname.length() + 1;
	if (Len > MaxLen)
		MaxLen = Len;
	return MaxLen;
}

void LetterQueue::PrintTable(int RL)
{
	LetterQueue* C = this;
	int LeftWidth = FindMaxLen(),
		RightWidth = std::to_string(RL).length(), 
		TmpLen = 0;

	if ((C != nullptr && C->mNext == nullptr && C->mLetter.mStudent.mName == "Aa") || (C == nullptr))
	{
		std::cout << "Table is empty\n";
		return;
	}

	for (int i = 0; i < LeftWidth + 2 + RightWidth + 2 + 3; i++)
		std::cout << "_";
	std::cout << std::endl;

	while (C != nullptr)
	{
		TmpLen = C->mLetter.mStudent.mName.length() + C->mLetter.mStudent.mSurname.length() + 1;
		std::cout << "| " << C->mLetter.mStudent.mSurname << " " << C->mLetter.mStudent.mName;

		for (int i = 0; i < LeftWidth - TmpLen; i++)
			std::cout << " ";

		std::cout << " | " << C->mLetter.mReport;
		
		for (int i = 0; i < RightWidth - std::to_string(C->mLetter.mReport).length(); i++)
			std::cout << " ";
		
		std::cout << " |\n";
		C = C->mNext;
	}

	for (int i = 0; i < LeftWidth + 2 + RightWidth + 2 + 3; i++)
		std::cout << "_";
	std::cout << std::endl;

}


void LetterQueue::LoadTable(EquatList *EL)
{
	std::string Names[13] = { "James", "Mary", "John", "Patricia", "Robert", "Jennifer", "Michael", "Linda", "William", "Elizabeth", "David", "Susan", "Joseph" },
		Surnames[13] = { "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson", "Taylor" };

	for (int i = 0; i < 13; i++)
	{
		Student *S = new Student(Names[i], Surnames[i], (double)rand() / (double)RAND_MAX);
		if (i % 3 == 0)
			S->mGoodness = 0;
		if (i % 5 == 0)
			S->mGoodness = 1;

		Push(*S, EL);
	}
}

void LetterQueue::UpdateTable(EquatList* EL, LetterQueue* LQ)
{
	t_compl X1, X2;
	LetterQueue* C = this;

	if (EL == nullptr || LQ == nullptr)
		return;

	while (!(C->mLetter.mStudent.mSurname == LQ->mLetter.mStudent.mSurname && C->mLetter.mStudent.mName == LQ->mLetter.mStudent.mName) && C != nullptr)
		C = C->mNext;
	while (C != nullptr && LQ->mLetter.mStudent.mName != "Aa")
	{
		EquatList* Tmp = EL;
		
		while (Tmp->mNext != nullptr)
		{
			C->mLetter.mReport += LQ->mLetter.mStudent.SolveEquation(&X1, &X2, Tmp->mEquation);
			Tmp = Tmp->mNext;
		}
		C->mLetter.mReport += LQ->mLetter.mStudent.SolveEquation(&X1, &X2, Tmp->mEquation);

		LQ->Pop();
		C = C->mNext;
	}
}

int LetterQueue::FindStudent(Student* S)
{
	LetterQueue* C = this;

	while (!(C->mLetter.mStudent.mSurname == S->mSurname && C->mLetter.mStudent.mName == S->mName) && C != nullptr)
		C = C->mNext;

	if (C != nullptr)
	{
		S->mGoodness = C->mLetter.mStudent.mGoodness;
		return 1;
	}

	return 0;
}
