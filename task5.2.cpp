# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# include <unistd.h>
#include "iostream"
#include "fstream"
#include <sstream>
#include <fcntl.h>
using namespace std;

//===========================================================================


/*

Дополнить программу возможностью выполнения команд с направлением потока вывода в файл (а не на экран, как это было ранее) возможностью корректного выполнения команд cd (смена рабочего каталога) и echo (вывод строки на экран).

Программа должна уметь выполнять команды вида:

·     команда [парам 1] [парам 2] … [парам N] > имя_файла;

·     cd имя_каталога;

·     echo [строка].

*/



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

		bool redirect_output = false;

		char* file_name = NULL;

		while(mas[index] != NULL)

		{

			mas[++index] = strtok(NULL, " ");

		}	

		for (int i = 0; mas[i] != NULL; i++) 

		{

			if (strcmp(mas[i], ">") == 0)

			{

				mas[i] = NULL; // vticaem NULL vmesto ">" 	

				file_name = mas[i + 1]; // otlo3ili ima faila v peremennuy

				redirect_output = true;

			}

		}			



		if (strcmp(buf,"cd")==0) // esli commanda cd

		{

			chdir(mas[1]);  // menyaem catalog	

		}

		else if (strcmp(mas[0], "echo") == 0) 

		{

		  for (int i = 1; mas[i] != NULL; i++) 

		  {

			std::cout << mas[i] << " ";

		  }

		}  		

		//for(int i=0; i <= index; i++)

		//	printf("mas[%d] = %s \n",i,mas[i]);

		if (fork() == 0)//otdelni process

		{			

			if (redirect_output)// otkrivaem fail i perenapravit potok vivodals

			  {

				int fd = open(file_name, O_WRONLY|O_CREAT|O_TRUNC, 0600);

				dup2(fd, 1); /*дублируем дескриптор (на стандартный вывод)*/

				close (fd); /*старый дескриптор больше не нужен*/ 

			  }

			execvp(mas[0],mas);

			cout << "error" << endl;

			return 0;

		}

		wait(0);//3dem process

	}		

	return 0;

}