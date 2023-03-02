# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
#include <sstream>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# include <unistd.h>
#include "iostream"
#include "fstream"
using namespace std;

/*1.   Сделать бесконечный цикл запросов строк на ввод. Придумать собственное слово для выхода (например, “by”). Когда пользователь вводит это слово — цикл заканчивается, программа завершается.
2.   Все остальные строки, введенные пользователем на каждой итерации цикла, считаются предложениями и подлежат разбору на отдельные слова (команда + ее параметры через пробел). Для разбора использовать функцию strtok().
3.   Пример такого разбора: из строки "ls -l -a" должен получиться такой массив строк вида:
"ls"  — команда;
"-l"   — парам 1;
"-a"  — парам 2;
(null)  — конец массива.
Проследите, что последний элемент массива строк равен null, т.к. системный вызов exec не знает размер передаваемого массива и считывает его до первого попавшегося null-а.
4.   Когда разбор строки на слова закончен и протестирован (!), создаем дочерний процесс — fork() и заменяем его код кодом той команды, которую ввел пользователь.
5.   Если команда не выполнилась (exec закончился неудачей) — выдать сообщение об ошибке, что команда не распознана.
*/


//===========================================================================



int main(int argc, char *argv[])

{

	//int N = atoi(argv[1]);

	cout<< "Started myTerminal. Hello, Jedi" << endl;

	

	while(true)

	{

		char buf[255];

		cout << "Enter command" << endl;

		cin.getline(buf,255); // anti space

		if (strcmp(buf,"by-by")==0) break; //strcmp: sravnivaem storki, a ne ukazateli

		//cout << buf << endl;

		

		char * mas[20];

		int index = 0;

		mas[index] = strtok(buf, " ");

		while(mas[index] != NULL)

		{

			mas[++index] = strtok(NULL, " ");

		}

		if (strcmp(buf,"cd")==0) // esli commanda cd

		{

			chdir(mas[1]);  // menyaem catalog

			

		}

		

		if (strcmp(buf,">")==0) // esli menyaem vivod

		{

			  // vticaem NULL vmesto ">"

			  // otlo3ili ima faila v peremennuy

			  // otkrivaem fail i perenapravit potok vivodals

			

		}

		

		//for(int i=0; i <= index; i++)

		//	printf("mas[%d] = %s \n",i,mas[i]);

		if (fork() == 0)//otdelni process

		{

			execvp(mas[0],mas);

			cout << "error" << endl;

			return 0;

		}

		

		wait(0);//3dem process

		

	}		

		

	return 0;

}
using namespace std;

//1.   Взять с командной строки количество создаваемых процессов.

//2.   Запустить цикл (от 0 до N), в котором вызвать функцию fork().

//3.   Дочерние процессы должны доложиться о себе и заснуть на короткое время — sleep(5).

//После пробуждения доложиться о том, что проснулись.

//4.   И родительский, и дочерние процессы должны выводить свои PID и PPID — getpid() и getppid() соответственно.

//5.   Дочерние процессы не пускать (!!!) на новую итерацию цикла.



int main(int argc, char *argv[])

{

	stringstream convert(argv[1]); // создаем переменную stringstream с именем convert, инициализируя её значением argv[1] 

	int count;

	if (!(convert >> count)) // выполняем конвертацию

		count = 0; // если конвертация терпит неудачу, то присваиваем myint значение по умолчанию

	cout<< "Process count = "<< count <<endl;

		

	cout<< "I'm parent. My PID is "<< getpid() << ", and my PPID is " << getppid() <<endl;

	int parentPid = getpid();



	

	for (int i = 0; i<count; i++)

	{

		int pid, status;

		if (getpid() == parentPid)

		{

			if (!(pid = fork())) //child 

			{

			  cout<< i << " Child process start. My PID is "<< getpid() << ", and my PPID is " << getppid() <<endl;

			  sleep(5); /*приостанавливаем выполнение на 5 секунд*/

			  cout<< i << " Child process awake. My PID is "<< getpid() <<endl;

			  exit(5);

			}

			//if (WIFEXITED(status)) /*проверка статуса завершения процесса*/

			  //cout<< i << " Cтатус завершения равен "<<WEXITSTATUS(status)<<endl;

			

			//while (waitpid(pid, &status, WNOHANG) == 0)

			//{

			//  /*ожидание продолжается*/

			 // sleep(1);

			//}

		}	

	}	

	return 0;

}