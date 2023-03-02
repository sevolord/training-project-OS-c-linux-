#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include <sys/sem.h>

using namespace std;

/*
Изучить механизм работы синхронизации процессов в ОС UNIX при помощи семафоров.

Написать программу, которая двумя разными процессами строго по очереди (это важно!) увеличивает на единицу значение целочисленной переменной, находящейся в разделяемой памяти. То есть один процесс может увеличить только чётное значение этой переменной, другой — только нечётное.

Реализовать два варианта работы: с синхронизацией и без неё. Сравнить работу обоих вариантов.

*/



int shmid, semid;

int *shm;



int main(int argc, char *argv[])

{

  // инициализируем генератор сл. чисел текущем временем

  srand(time(NULL));

	// вызов ./a.out [mode] [max value]

    // mode = 0 - без семафора 1 - с  семафором

	// max value - значение, до которого будет итерирвоаться переменная 

  int mode = atoi(argv[1]);

  int maxValue = atoi(argv[2]); 

  cout << "mode of the program: ";

  mode==1? cout <<"for with semaphores" << endl: cout <<"without semaphores"<< endl;

  cout << "maximum value for the shared variable:" << maxValue << endl;

  

  // Create a shared memory segment with an IPC_PRIVATE key and access rights 0666

  shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);



  // Attach the shared memory segment to the process's address space

  shm = (int *)shmat(shmid, NULL, 0);



  // Initialize the shared variable to 0

  *shm = 0;



  // Create two child processes

   

  pid_t p1 = fork();





  if (p1 < 0 ) {

    // If there was an error creating the child processes, print an error message and exit

    cout << "Error creating child processes" << endl;

    exit(1);

  }





    int attempts = 0; // Counter for the number of attempts to increase the shared variable



    if (mode == 0) 

	{

      // Without semaphores

	  if (p1 == 0){

		  while (*shm < maxValue) {

			// Check if the shared variable is even or odd

			if (*shm % 2 == 0) {

			  // If it is even, increase it by 1

			  (*shm)++;

			  attempts++;

			  cout << "p1 even variable = "<< *shm-1 <<" increase it by 1, pid = " << getpid() << endl;

			}

			else 

				cout << "p1 odd variable = "<< *shm <<" pid = " << getpid() << endl;



			// Sleep for a random number of seconds

			int sl = rand() % 3;

			cout << "sleep "<< sl  << endl;

			sleep(sl);

		  }

	  }else{

		  while (*shm < maxValue) {

			// Check if the shared variable is even or odd

			if (*shm % 2 == 1) {

			  // If it is odd, increase it by 1

			  (*shm)++;

			  attempts++;

			  cout << "p0 odd variable = "<< *shm-1 <<" increase it by 1, pid = " << getpid() << endl;

			}

			else 

				cout << "p0 even variable = "<< *shm <<" pid = " << getpid() << endl;



			// Sleep for a random number of seconds

			int sl = rand() % 3;

			cout << "sleep "<< sl  << endl;

			sleep(sl);

		  }

	  }

    } 

	else 

	{

      // With semaphores

		// Create a set of two semaphores with an IPC_PRIVATE key and access rights 0666

		semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);



		// Initialize the value of the first semaphore to 1 and the second semaphore to 0

		semctl(semid, 0, SETVAL, 1);

		semctl(semid, 1, SETVAL, 0);



		while (*shm < maxValue) {

		  // Decrease the value of the other process's semaphore to "lock" the critical section

		  if (getpid() == p1) {

			// If this is the first child process, decrease the value of the second semaphore

			struct sembuf sops = {1, -1, 0};

			semop(semid, &sops, 1);

		  } else {

			// If this is the second child process, decrease the value of the first semaphore

			struct sembuf sops = {0, -1, 0};

			semop(semid, &sops, 1);

		  }



		  // Increase the value of the shared variable

		  cout << "variable = "<< *shm <<" increase it by 1, pid = " << getpid() << endl;

		  (*shm)++;

		  attempts++;



		  // Increase the value of the other process's semaphore to allow it to access the critical section

		  if (getpid() == p1) {

			// If this is the first child process, increase the value of the second semaphore

			struct sembuf sops = {1, 1, 0};

			semop(semid, &sops, 1);

		  } else {

			// If this is the parent process, increase the value of the first semaphore

			struct sembuf sops = {0, 1, 0};

			semop(semid, &sops, 1);

		  }



		  // Sleep for a random number of seconds

		  int sl = rand() % 3;

		  cout << "sleep "<< sl  << endl;

		  sleep(sl);

		}

	}

	// Wait for both child processes to finish

	wait(NULL);

	wait(NULL);



	// Detach the shared memory segment from the process's address space

	shmdt(shm);



	// Delete the shared memory segment

	shmctl(shmid, IPC_RMID, NULL);



	// Delete the set of semaphores

	semctl(semid, 0, IPC_RMID);



	// Print the number of attempts to change the shared variable for each process

	cout << "Process " << getpid() << " made " << attempts << " attempts to change the shared variable" << endl;



	return 0;

}



