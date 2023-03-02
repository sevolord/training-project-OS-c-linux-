/*Программа создает 3 дочерних процесса и запускает на них 3 разных алгоритма сортировки. 

Алгоритмы приложены в файле ниже (или воспользуйтесь поисковой системой Интернет самостоятельно).

Что запрограммировать:

1.   Взять с консоли длину массива.

2.   Создать массив указанной длины, заполнить его псевдослучайными числами — rand(), записать его в файл — open(), dprintf(), close().

3.   Запустить 3 дочерних процесса, по одному на каждую сортировку.

4.   В коде дочерних процессов:

1)   вызвать функцию сортировки для созданного массива;

2)   записать в файл уже отсортированный массив;

3)   вызвать функцию times(), передав ей структурную переменную типа tms;

4)   из полей структуры взять tms_utime и tms_stime, сложить их и вывести время работы процесса.

5.   Родительский процесс должен подождать все дочерние процессы.

*/







# include <sys/types.h>

# include <sys/wait.h>

# include <unistd.h>

#include "iostream"

#include "fstream"

#include <sstream>

#include <sys/types.h>

#include <sys/times.h>



using namespace std;





//************************** Bubble Sort **************************************

void bubble (int * v, int size) {

 for (int i=0; i<size; i++){

  for (int j=0; j<size-1; j++){

   if (v[j]>v[j+1]){

    int tmp = v[j];

    v[j] = v[j+1];

    v[j+1] = tmp;

   }

  }

 }

}



//************************** Shell Sort ***************************************

void shell (int * v, int size) {

 int s=0;

 int j=0;

 do {

  s = 3*s+1;

 }while (s<=size);

 do {

  s = (s-1) / 3 + 1;

  for (int i=0; i<size; i++){

   j=i-s;

   int tmp = v[j+s];

   while (j >= 0 && tmp < v[j]) {

    v[j+s] = v[j];

    j -= s;

   }

   v[j+s] = tmp;

  }

 }while (s!=1);

}



//************************** Quick Sort ***************************************

void qs (int * v, int low, int high) {

 int tmp=0;

 int  l = low;

 int r = high;

 int m = v[(l+r) / 2];

 do {

   while (v[l]<m) l++;

   while (v[r]>m) r--;

   if (l<=r){

    tmp = v[l];

    v[l] = v[r];

    v[r] = tmp;

    l++;

    r--;

   }

 }while (l<r);

 if (l<high) qs(v, l, high);

 if (r>low) qs(v,low, r);                                                                                             

}



void quicks (int * v, int size){

  qs (v, 0, size - 1);

}



//------------------------ Save to File -------------------------------------

void save2file (char * fname, int * v, int size) {



  ofstream OutFile(fname);

  for (int i=0; i<size; i++)

    OutFile << v[i] << endl;

  OutFile.close();

}



//===========================================================================



int main(int argc, char *argv[])

{

	int N = atoi(argv[1]);

	int mas[N];

	

	

	for(int i=0;i<N;i++)

	{

		mas[i] = rand() % 1000;

		//cout<< mas [i] <<endl;

	}

	save2file("arr.txt",mas,N);

	

	if (fork() == 0)

	{	  	

		cout<< "Bubble start" <<endl;

		bubble(mas,N);

		struct tms tms_struct;

		times(&tms_struct);

		cout<< "Bubble finish " << "Total time: " << (tms_struct.tms_utime + tms_struct.tms_stime) <<endl;		

		//for(int i=0;i<N;i++)

		//	if(N<=10) cout<< mas [i] <<endl;	

		save2file("bubbleArr.txt",mas,N);		

		return 0;

	}

	

	if (fork() == 0)

	{

		cout<< "qs start" <<endl;

		quicks(mas,N);

		struct tms tms_struct;

		times(&tms_struct);

		cout<< "qs finish " << "Total time: " << (tms_struct.tms_utime + tms_struct.tms_stime) <<endl;		

		//for(int i=0;i<N;i++)

		//	if(N<=10) cout<< mas [i] <<endl;

		save2file("quicksArr.txt",mas,N);			

		return 0;

	}	

	

	if (fork() == 0)

	{

		cout<< "shell start" <<endl;

		shell(mas,N);

		struct tms tms_struct;

		times(&tms_struct);

		cout<< "shell finish " << "Total time: " << (tms_struct.tms_utime + tms_struct.tms_stime) <<endl;			

		//for(int i=0;i<N;i++)

		//	if(N<=10) cout<< mas [i] <<endl;

		save2file("shellArr.txt",mas,N);			

		return 0;

	}	

		

		

	return 0;

}