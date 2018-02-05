
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include <dos.h>

// ВЫбор типа сортировки
// True - сортировка по возрастанию
// False - по убыванию
bool tsort (void) {
	printf("Выбор типа сортировки\n");
	printf("1 - По возростанию \n");
	printf("2 - По убыванию \n");
	char character = getchar();
	getchar();
	return character == '1';
}

// Возвращает результат сравнения операндов
// в зависимости от типа сортировки
// l - левый операнд
// r - правый операнд
// downup - тип сортироваки
bool getResult (long long l, long long r, bool downup) {
	if (downup) return l > r;
	else return l < r;
}

// Сортировка Хоара
void hoar (long long *file_sizes, char **file_names, int first, int last, bool pr)
{
	int i = first;
	int j = last;
	long long x = file_sizes[(first + last) / 2];
	char* buffer;
	long long tmp;

	do {
		while (getResult(x, file_sizes[i], pr)) i++;
		while (getResult(file_sizes[j], x, pr)) j--;

		if(i <= j) {
			if (i < j) 
			{
				tmp = file_sizes[j];
				file_sizes[j] = file_sizes[i];  	
				file_sizes[i] = tmp;


				buffer = file_names[j];
				file_names[j] = file_names[i];  	
				file_names[i] = buffer;
			}
				i++;
				j--;
			
		}
	} while (i <= j);

	if (i < last)
		hoar (file_sizes, file_names, i, last, pr);
	if (first < j)
		hoar (file_sizes, file_names, first, j, pr);
}

// Печать  массивов
void print(long long*file_sizes, char **file_names, int first, int last)
{
	int k;		// Для первых 10 строк
	int j;		// Для последних 10 строк
	
	if(last < 20) {
		k = last-1;
		j = last+1;
	}
	else {
		k = 10;
		j = last - 11;
	}

    for(int i=first; i<k; i++)
		printf("%s     %lld\n", file_names[i], file_sizes[i]);

	printf("------------------------------------------\n");

	
	for(int i=j; i<last-1; i++)
		printf("%s     %lld\n", file_names[i], file_sizes[i]);
	printf("==========================================\n");
}



void main()
{
	
	printf("Now on GitHub! \n");

	int action = -1;
	struct _finddata_t c_file;
	intptr_t hFile;
	char **file_names = NULL;
	long long *file_sizes = NULL;
	int count_of_files=0;
	char* buffer;
	long long x;
	//char m_path[50] = "c:\\intel\\logs\\*.*";
	char m_path[50];
	char character = 'a';
	clock_t start, stop;
	bool pr;
	double ff = CLOCKS_PER_SEC;
	double dt;

	setlocale(LC_ALL, "Russian");
	clock ();

	while(character!='0')
	{
		printf("1 - Ввод каталога\n");
		printf("2 - Сортировка вставками\n");
		printf("3 - Сортировка пузырьком\n");
		printf("4 - Сортировка Хоара\n");
		printf("5 - Сортировка выбором\n");
		printf("0 - Завершение работы\n");
		printf("Введите номер действия: ");
		character = getchar();
		getchar();
		
		switch(character)
		{
		case '0': printf("Программа завершена");
			break;
		case '1':
			
			printf("Введите каталог: ");

			gets(m_path);

			strcat(m_path,"\\*.*");
			
			printf("Каталог -> %s \n",m_path);
			if ((hFile=_findfirst(m_path, &c_file))==-1L)		// Найти первый файл в каталоге
				printf("no file in current directory!\n");
			else
			{
				do
				{
					// Выделяем память для расширения массива указателей на имя файла
					file_names=(char**)realloc(file_names, (count_of_files+1)*sizeof(char*));
					// Выделяем память для имени файла
					file_names[count_of_files]=(char*)malloc(strlen(c_file.name) + 1);
					// Крпируем имя файла из c_file.name  в выделенную область
					strcpy(file_names[count_of_files], c_file.name); 
					// Выделяем память для расширения массива размеров файла
					file_sizes=(long long*)realloc(file_sizes, (count_of_files+1)*sizeof(long long));
					file_sizes[count_of_files]=c_file.size;

					count_of_files++;
				}
				while(_findnext(hFile, &c_file)==0);		// Найти следующий файл в каталоге
				_findclose(hFile);							
				print(file_sizes, file_names, 0, count_of_files);
			}

			break;
		case '2':
			// Сортировка вставками

			// Получаем тип сортировки
			pr = tsort();
			// Запускаем часы
			start = clock ();
			for (int i=1; i < count_of_files; i++)
			{  
                for (int j=i-1; j>=0 && getResult(file_sizes[j], file_sizes[j+1], pr); j--)
				{
					
					x = file_sizes[j+1];
					file_sizes[j+1] = file_sizes[j]; 
					file_sizes[j] = x;

					buffer = file_names[j+1];
					file_names[j+1] = file_names[j];  	
					file_names[j] = buffer;

				}
			}
			// Останавливаем часы
			stop = clock();
			print(file_sizes, file_names, 0, count_of_files);
			printf("Время %f секунд\n", (stop - start) / ff);
			break;
		case '3':
			// Пузырьковая сортировка

			// Получаем тип сортировки
			pr = tsort();
			start = clock ();
			for(int i=0; i<count_of_files - 1;i++)
			{
				for(int j=i+1; j<count_of_files; j++)
				{
					if(getResult(file_sizes[i],file_sizes[j],pr))
					{
						x = file_sizes[j];
						file_sizes[j] = file_sizes[i];  	
						file_sizes[i] = x;

						buffer = file_names[j];
						file_names[j] = file_names[i];  	
						file_names[i] = buffer;
					}

				}
			}
			stop = clock();
			print(file_sizes, file_names, 0, count_of_files);
			
			printf("Время %f секунд\n", (stop - start) / ff);
			break;
		case '4':
			// Сортировка Хоара
			pr = tsort();
			start = clock ();
			hoar (file_sizes, file_names, 0,  count_of_files-1, pr);
			stop = clock();
			print(file_sizes, file_names, 0, count_of_files);
			printf("Время %f секунд\n", (stop - start) / ff);
			break;
		case '5':
			// Сортировка выбором
			pr = tsort();
			start = clock ();
			
			for (int i = 0; i < count_of_files -1; i++) 
			{
				int min_i = i;
				
				for (int j = i + 1; j <count_of_files; j++)
				{
					if (getResult(file_sizes[min_i], file_sizes[j], pr)) 
					{
						min_i = j;
					}
				}

				x = file_sizes[i];
				file_sizes[i] = file_sizes[min_i];
				file_sizes[min_i] = x;

				buffer = file_names[min_i];
				file_names[min_i] = file_names[i];  	
				file_names[i] = buffer;
				
			}
			stop = clock();
			print(file_sizes, file_names, 0,count_of_files);
			
			printf("Время %f секунд\n", (stop - start) / ff);
			break;
	}

  }
}

