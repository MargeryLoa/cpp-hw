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
	std::cout << "d - Удаление уравнения из списка(сверху)" << std::endl;

}


int main()
{
	EquatList* TestE = new EquatList();
	LetterQueue* NewLetters = new LetterQueue();
	LetterQueue* ReportTable = new LetterQueue();

	std::string *FileName = new std::string("");
	Student* S = new Student();
	std::string Name, Surname;
	double G;

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
			std::cin >> Name;
			std::cout << "Введите фамилию студента: \n";
			std::cin >> Surname;
			std::cout << "Введите ожидаемую успеваемость студента(0 до 1): \n"; 
			std::cin >> G;
			S = new Student(Name, Surname, G);

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
			ReportTable->CheckOneStudent(TestE);
			break;
		case 'd':
			TestE->Remove();
			TestE->Print();
			break;
		}

		std::cin >> c;
		
	}
}
