#include <iostream>
#include <vector>

#include "student.h"

#pragma once

class Letter
{
public:
	Student mStudent;
	int mSize, mReport;
	
	Letter(void);
	Letter(Student S, int NoE);

	void Print(void);
};