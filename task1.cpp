# include <sys/types.h>

# include <sys/wait.h>

# include <unistd.h>

#include "iostream"

#include <sstream>

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