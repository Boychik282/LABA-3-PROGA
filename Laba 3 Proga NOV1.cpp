//Попов А.Д Вариант 42
// Задание 8
// В матрице сформировать массив, состоящий из НОМЕРОВ строк, в которых сумма элементов меньше или равна заданной величине А.
//Найти минимальный элемент в этих строках и прибавить его ко всем элементам матрицы.
//Проверено 05 06 2024

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<string.h>
#include<windows.h>
#include<iomanip>
#include<conio.h>
#include <cstdlib>// fin >> выводим данные из потока куда то
#include <cstring> // fin << вводим данные в поток откуда то
#include <ctype.h>
#include <limits>

using namespace std;
typedef double elemType;


int main()
{
	//int k;

	enum class ProcessingResult {
		Continue, // продолжить обработку
		Error_Schitivanie_Faila,    // произошла ошибка открытия файла
		Matrica_NE_Pryamougolnaya,   // данные некорректны
		Oshibka_Videlenia_Pamati,
		Error_Nepravilniy_Symvol

	}; ///////////////////////////// - названия констант не интуитивные

	ProcessingResult result = ProcessingResult::Continue;
	while (true) {
		SetConsoleOutputCP(1251);
		SetConsoleCP(1251);
		string file_name; //Имя файла
		cout << endl << endl;
		cout << "Введите название файла или * для завершении программы: " << endl;
		getline(cin, file_name);
		system("CLS");
		if (file_name == "*")
		{
			system("CLS");
			char ch;
			cout << "Желаете завершить работу программы? (Введите " "\"" << "Y" << "\"" " или " "\"" << "y" << "\"" ", если вы хотите заверишть работу программы)" << '\n' <<
				"Если вы хотите продолжить работу программы, введите любую другую букву";
			cin >> ch;
			cin.ignore();
			system("CLS");
			if (ch == 'y' || ch == 'Y') {
				cout << "Завершение работы программы" << endl<<endl;
				return 0;
			}
			else {
				cin.ignore();
				continue;
			}
		}
		ifstream inputFile(file_name); //открытие потока
		if (!inputFile.is_open())// проверка открытия 
		{
			cout << "Файл \"" << file_name << "\"" << "не был открыт, код ошибки: " << errno << endl;
			perror("Ошибка открытия файла");
			cout << endl << endl;
			continue;
		}
		string pr; //проверка на корректность, ,без stringstream


		// 1 способ хранения матрицы
		int position; //  position - позиция в файле для возврата		
		bool error = false; // задаём ошибку,как false
		char err;

		double d;
		int m = 0, n = 0, k = 0, num_min = 999999999;//len_str;
		char c = 0;
		bool flg = true;//для возвращения внешнего цикла


		while (flg) {
			char pos_user = inputFile.peek();
			//position = inputFile.tellg();
			inputFile >> d; // Считываем матрицу по числам d

			// Увеличиваем счетчик элементов
			if (inputFile.fail() && !inputFile.eof()) {
				cout << endl;
				pr = pos_user;
				result = ProcessingResult::Error_Schitivanie_Faila;
				break;
			}

			n++;
			while (c != EOF) {
				c = inputFile.get();
				switch (c) {
				case ' ':
				case '\t':
					break;
				case EOF:
					flg = false;
					result = ProcessingResult::Continue;
				case '\n':
					if (n) {
						num_min = min(num_min, d); // Перенести вычисление минимума после вывода матрицы на экран

						// можно посчитать кол-во эл-тов в 1-й строке и сравнивать с ост
						if (m != 0) {
							if (k != n) {
								result = ProcessingResult::Matrica_NE_Pryamougolnaya;
								break;
							}
						}
						else
							k = n;
						m++;
						n = 0;
					}
					break;
				default:
					inputFile.unget(); // Возвращает символ, который был успешно отправлен назад, используется для указания на ошибку.
					c = EOF;
				}
			}
			c = 0;
		}

		//m - строки, n - столбцы 
		n = k;
		k = m * n;

		if (inputFile.peek() == EOF && !m && !n)
		{
			cout << "Вы открыли пустой файл" << endl;
			_getch();
			system("cls");
			inputFile.close();
			continue;
		}
		//Проверка матрицы

		////////////////////////////// - где проверки на возможно обнаруженную ошибку в матрице? где проверка на отсутствие матрицы? какой смысл пытаться выделять память, не проверив всё это?

		cout << "Количество строк: " << m << '\t' << "Количество столбцов: " << n << '\t' << "Количество элементов в матрице: " << k << '\n' << endl;
		double** matr;
		//с nothrow 
		matr = new (nothrow) double* [m]; //nothrow сообщает компилятору, что и объявленная функция, и функции, которые она вызывает, никогда не создают исключений
		int i = -1;
		if (matr)
			for (i = 0; i < m; i++)
			{
				matr[i] = new(nothrow) double[n];
				if (!matr[i]) {
					//////проверено  ///////////////////////////////// - зачем?
					result = ProcessingResult::Oshibka_Videlenia_Pamati; ///////////////////////////////// - зачем? (см. ниже)
					break;
				}

			}

		/*if (i != m)
		{
			for (int k = 0; k < i; k++) {
				delete[]matr[k];
			}
			delete[]matr;
			cout << "Не удалось Выделить место под все строки";
			/////////////// сообщение об ошибке
			_getch();
			result = ProcessingResult::Invalid1; ///////////////////////////////// - зачем? (см. ниже)
			///continue
		}*/
		//cin >> k;
		//cin.ignore();
		//if (k == 1) {
			//result = ProcessingResult::Invalid1;
		//}

		switch (result) {
		case ProcessingResult::Continue:

			system("CLS");
			cout << "Программа отработала штатно" << endl << endl;
			// Продолжить обработку
			break;
		case ProcessingResult::Error_Schitivanie_Faila:
			delete[]matr;
			system("CLS");
			///////////////////////////////////// - ??? уже всё д.б. проверено ранее, и result==ProcessingResult::Error здесь д.б. достаточно; упростите
			cout << "Ошибка считывания \n" << m + 1 << "-ой строки\n" << n << "-ого столбца " << "\nСимвол,который создал ошибку обработки матрицы : " << "\"" << pr << "\"" << endl;																							//"\nПозиция некорректного символа: "<< position << endl;          // позиция указателя (где чтение файла остановилось) в файле - длинна значения + 1 
			_getch();
			inputFile.close();
			continue;
		case ProcessingResult::Matrica_NE_Pryamougolnaya: ////////////////////////// - Вы эту же конст-ту используете для обозн-я непрямоуг-ти! Где обработка непрямоуг-ти?
			delete[]matr;
			system("CLS");
			cout << "Матрица не является прямоугольной";
			_getch();
			// Обработать некорректные данные
			continue; 
			inputFile.close();
		case ProcessingResult::Oshibka_Videlenia_Pamati: ////////////////////////////// - эта конст-та и Invalid вообще избыточны (не нужны) для ошибки выделения памяти, т.к. в ней для идентификации шага, на котором возникла ош-ка, достаточно уже имеющейся i и пустого указателя в matr
			delete[]matr;
			system("CLS");
			cout << "Ошибка выделения памяти";
			_getch();
			// Обработать некорректные данные
			continue; 
			inputFile.close();
		}

		inputFile.clear();
		inputFile.seekg(0);
		//заполнение матрицы

		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				inputFile >> matr[i][j];

			}

		}  ////////////////////////////////// - поток!
		inputFile.close();
		double A;
		cout << "Текущая  матрица:" << '\n';
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				cout << matr[i][j] << " ";  ////////////////////////////////// - форматированный вывод! симв. литерал!
			}
			cout << endl;
		}
		//		num_min = min(num_min, d);
		//		cout << num_min << endl;
				//while (true) {
		cout << "Введите заданную величину A: " << '\n';
		while (true) {
			cin >> A;
			if (cin.fail()) {
				cout << "Введенное значение не является числом. Пожалуйста, введите допустимое значение." << endl;
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			}
			else if (cin.peek() != '\n') {
				cout << "Введенное значение содержит недопустимые символы. Пожалуйста, введите допустимое число." << endl;
				cin.clear();
				cin.ignore((numeric_limits<streamsize>::max)(), '\n');
			}
			else {
				cout << "\nЗначение A введено верно \n";
				break;
			}
		}


		//}
		int minElement;
		bool net_strok_udov_usl = true;
		double sum = 0;
		// Формирование массива номеров строк, в которых сумма элементов меньше или равна A
		int* rows = new int[m]; // Динамическое выделение памяти для массива
		int num_rows = 0;
		for (int i = 0; i < m; i++) {
			rows[i] = 0; ///
			double sum = 0;
			for (int j = 0; j < n; j++) {
				sum += matr[i][j];
			}
			if (sum <= A && num_rows < m) {
				net_strok_udov_usl = false;
				rows[num_rows++] = i;
			}
		}
		if (net_strok_udov_usl == true)//////////////// не работает
		{
			minElement = 0;
			cout << "Нет строк в которых сумма элементов меньше либо равна A";
			cout << endl;

		}
		else {
			cout << "Номера строк, в которых сумма элементов меньше или равна A:" << endl;
			for (int i = 0; i < num_rows; i++)
			{
				cout << rows[i] + 1 << " "; // Добавляем 1, чтобы отображать номера строк с единицы
				cout << endl;
			}
			// Поиск минимального элемента в этих строках
			minElement = (std::numeric_limits<int>::max)(); // Инициализируем минимальный элемент максимальным возможным значением
			for (int i = 0; i < num_rows; i++) {
				for (int j = 0; j < n; j++) {
					minElement = min(minElement, matr[rows[i]][j]);
				}
			}
		}
		
		// Прибавление минимального элемента ко всем элементам матрицы
		if (sum <= A && num_rows < m) {
			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					matr[i][j] += minElement;
				}
			}
		}
		else {
			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					matr[i][j];
				}
			}
		}

		size_t* maximalnaya_dlina = new (nothrow) size_t[n];
		if (!maximalnaya_dlina)
		{
			for (int h = 0; h < i; h++)
				delete[]matr[h];
			delete[]matr;
			delete[]rows;
			inputFile.close();
			cout << "Ошибка выделения памяти для форматированного вывода" << endl;
			_getch();
			system("cls");
			continue;
		}

		for (size_t i = 0; i < n; i++)
			maximalnaya_dlina[i] = 0;
		for (size_t i = 0; i < m; i++)
		{
			for (size_t j = 0; j < n; j++)
			{
				ostringstream vivod;
				vivod << matr[j][i];
				size_t len = vivod.tellp();
				maximalnaya_dlina[i] = max(len, maximalnaya_dlina[i]);

			}
		}
		if (sum <= A) {
				rows[num_rows++] = i;
			}
		// Вывод измененной матрицы
		cout << "Измененная матрица:" << '\n' << endl;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				cout << fixed << setprecision(0) << setw(maximalnaya_dlina[j] + 1) << matr[i][j] << " "; // Символьный литерал для форматированного вывода
			}
			cout << endl;
		}
		cout << endl;
		
		// Очистка памяти
		for (int i = 0; i < m; i++) {
			delete[] matr[i];
		}

		delete[] matr;
		//delete[] rows;

		system("pause");
		cin.ignore();
		system("CLS");
	}
	
	return 0; //////////////////////////////////// - нет - как в л.р. №1; + РЕСУРСЫ!!!
}
