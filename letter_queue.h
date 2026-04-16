#include <iostream>
#include "letter.h"
#include "equation_list.h"

#pragma once
class LetterQueue
{
	Letter mLetter;
public:
	LetterQueue* mNext = nullptr;

	LetterQueue() = default;
	LetterQueue(Letter L);

	void Push(Student S, EquatList * EL);
	void Pop(); 
	
	void PrintList();
	int FindMaxLen();
	void PrintTable(int RL);
	void LoadTable(EquatList* EL);
	void UpdateTable(EquatList* EL, LetterQueue* LQ);
	
	int FindStudent(Student* S);
	void CheckOneStudent(EquatList* E);
	
	~LetterQueue();
	//	void ReadFile(std::string FileName);
};
