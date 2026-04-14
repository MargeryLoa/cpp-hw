#include <iostream>
#include "letter.h"
#include "equation_list.h"

#pragma once
class LetterQueue
{
public:
	Letter mLetter;
	LetterQueue* mNext;

	LetterQueue(void);
	LetterQueue(Letter L);

	void Push(Student S, EquatList * EL);
	void Pop(void); 
	
	void PrintList(void);
	int FindMaxLen(void);
	void PrintTable(int RL);
	void LoadTable(EquatList* EL);
	void UpdateTable(EquatList* EL, LetterQueue* LQ);
	
	int FindStudent(Student* S);
	//	void ReadFile(std::string FileName);
};