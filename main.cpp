#include <iostream>
#include <ctime>

#include "equation_list.h"
#include "letter_queue.h"

static void PrintMenu(void)
{
	std::cout << "0 - Выход" << std::endl;
	std::cout << "1 - Проверить новые письма" << std::endl;
	std::cout << "2 - Опубликовать успеваемость(по умолчанию до первой проверки у всех 0)" << std::endl;
	std::cout << "* Отладка" << std::endl;
	std::cout << "3 - Считать уравнения из файла" << std::endl;
	std::cout << "4 - Добавить студента в очередь" << std::endl;
	std::cout << "5 - Вывести список уравнений" << std::endl;
	std::cout << "6 - Вывести список студентов в таблице" << std::endl;
	std::cout << "7 - Удаление письма из очереди" << std::endl;
	std::cout << "8 - Размер списка уравнений" << std::endl;
	std::cout << "9 - Вывод списка писем" << std::endl;
	std::cout << "c - Очистка консоли" << std::endl;
	std::cout << "q - Посмотреть решение уравнения у студента" << std::endl;

}


int main(void)
{
	EquatList* TestE = new EquatList();
	LetterQueue* NewLetters = new LetterQueue();
	LetterQueue* ReportTable = new LetterQueue();

	std::string *FileName = new std::string("");
	Student* S = new Student();
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	PrintMenu();
	TestE->ReadFile("e1.txt");
	NewLetters->LoadTable(TestE);
	ReportTable->LoadTable(TestE);

	char c;
	std::cin >> c;
	while (c != '0')
	{
		switch (c)
		{
		case '0':
			break;
		case '1':
			ReportTable->UpdateTable(TestE, NewLetters);
			//ReportTable->PrintList();
			break;
		case '2':
			ReportTable->PrintTable(TestE->SizeOfList());
			break;
		case '3':
			std::cout << "Введите название файла(без .txt): ";
			std::cin >> *FileName;
			TestE->ReadFile(*FileName + ".txt");
			break;
		case '4':
			std::cout << "Введите имя студента: \n";
			std::cin >> S->mName;
			std::cout << "Введите фамилию студента: \n"; 
			std::cin >> S->mSurname;
			std::cout << "Введите ожидаемую успеваемость студента(0 до 1): \n"; 
			std::cin >> S->mGoodness;
			ReportTable->Push(*S, TestE);
			NewLetters->Push(*S, TestE);
			break;
		case '5':
			TestE->Print();
			break;
		case '6':
			ReportTable->PrintList();
			break;
		case '7':
			NewLetters->Pop();
			NewLetters->PrintList();
			break;
		case '8':
			std::cout << "В списке уравнений: " << TestE->SizeOfList() << std::endl;
			break;
		case '9':
			NewLetters->PrintList();
			break;
		case 'c':
			system("cls");
			PrintMenu();
			break;
		case 'q':
			EquatList * C = TestE;
			int NumE;
			std::cout << "Введите имя студента: \n";
			std::cin >> S->mName;
			std::cout << "Введите фамилию студента: \n";
			std::cin >> S->mSurname;
		
			if (ReportTable->FindStudent(S) == 0)
				std::cout << "Студент в списке не найден\n";
			else
			{
				t_compl X1, X2;

				std::cout << "Введите номер уравнения: \n";
				std::cin >> NumE;

				for (int i = 1; i < NumE && C != nullptr; i++)
					C = C->mNext;

				std::cout << "Найдено уравнение: \n" << C->mEquation.mA << " * x ^ 2 + (" << C->mEquation.mB << ") * x + (" << C->mEquation.mC << ")\n";

				if (S->SolveEquation(&X1, &X2, C->mEquation) == 0)
					std::cout << "Решено неверно\n";
				else
				{
					std::cout << "Решено верно\n";
					std::cout << X1.Re << " + i * " << X1.Im << std::endl;
					std::cout << X2.Re << " + i * " << X2.Im << std::endl;
				}
			}
			
			break;
		}

		std::cin >> c;
		
	}
}