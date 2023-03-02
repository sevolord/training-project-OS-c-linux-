/*

    Написать программу, оставляющую сообщения для других процессов операционной системы. 

	Сообщения должны быть в системе вне зависимости от жизненного цикла процесса, оставившего их.

	Использовать механизм межпроцессного взаимодействия — разделяемую память (shared memory).

	

	key = 345

	

    To create  ./a.out [key] c 

    To write   ./a.out [key] w 

    To read    ./a.out [key] r 

    To delete  ./a.out [key] d

*/



#include <iostream>

#include <string>

#include <sys/ipc.h>

#include <sys/shm.h>

#include <cstring>



using namespace std;



int main(int argc, char* argv[]) {

		// Check if the required number of arguments have been provided

		if (argc < 3) {

			cerr << "Error: Not enough arguments provided." << endl;

			return 1;

		}



		// Parse the shared memory key and mode from the command line arguments

		int shm_key = atoi(argv[1]);

		string mode = argv[2];

		

		// Check the mode and perform the appropriate action

		if (mode == "c") {

			// Режим «создание разделяемой памяти». Вызвать системный вызов shmget(). Если он вернул <0, то выдать сообщение об ошибке. 

			//При создании разделяемой памяти использовать параметры IPC_CREAT|0644 и ключ разделяемой памяти. Выдать сообщение о том, что разделяемая память создана. 

			//Завершить программу.

			int shm_id = shmget(shm_key, 1024, IPC_CREAT | 0644);

			if (shm_id < 0) {

				cerr << "Error: Failed to create shared memory segment." << endl;

				return 1;

			}

			cout << "Shared memory segment created." << endl;			

		} else if (mode == "w") {

			//Режим «записи сообщения в память». Вызвать системный вызов shmget(). Если он вернул <0, то выдать сообщение об ошибке. 

			//Объявить строковую переменную и соединить ее с разделяемой памятью системным вызовом shmat(). 

			//Все, что будет записано в эту строку,  сохранится в разделяемой памяти и не исчезнет. Записать передаваемое сообщение в эту строку. 

			//Отсоединить переменную от разделяемой памяти системным вызовом shmdt(). Завершить программу.

			

			int shm_id = shmget(shm_key, 1024, 0644);

			if (shm_id < 0) {

				cerr << "Error: Failed to attach to shared memory segment." << endl;

				return 1;

			}

			char* shm_ptr = (char*) shmat(shm_id, NULL, 0);

			if (shm_ptr == (char*) -1) {

				cerr << "Error: Failed to attach to shared memory segment." << endl;

				return 1;

			}	

			// Write the message to the shared memory segment

			string message = argv[3];

			strcpy(shm_ptr, message.c_str());



			// Detach from the shared memory segment

			shmdt(shm_ptr);

			cout << "Message written to shared memory." << endl;			

		} else if (mode == "r") {

			//Режим «чтение из разделяемой памяти». Вызвать системный вызов shmget(). Если он вернул <0, то выдать сообщение об ошибке. 

			//Объявить строковую переменную и соединить ее с разделяемой памятью системным вызовом shmat(). Вывести эту строковую переменную на экран. 

			//Отсоединить переменную от разделяемой памяти системным вызовом shmdt(). Завершить программу.



			int shm_id = shmget(shm_key, 1024, 0644);

			if (shm_id < 0) {

				cerr << "Error: Failed to attach to shared memory segment." << endl;

				return 1;

		}

		char* shm_ptr = (char*) shmat(shm_id, NULL, 0);

		if (shm_ptr == (char*) -1) {

			cerr << "Error: Failed to attach to shared memory segment." << endl;

			return 1;

		}

		// Read the message from the shared memory segment

		cout << "Message from shared memory: " << shm_ptr << endl;

		// Detach from the shared memory segment

		shmdt(shm_ptr);

		} else if (mode == "d") {

			//Режим «удаление разделяемой памяти». Вызвать системный вызов shmget(). Если он вернул <0, то выдать сообщение об ошибке. 

			//Для удаления разделяемой памяти использовать системный вызов shmctl(). Выдать сообщение о том, что разделяемая память удалена. Завершить программу.

	

			int shm_id = shmget(shm_key, 1024, 0644);

			if (shm_id < 0) {

				cerr << "Error: Failed to attach to shared memory segment." << endl;

				return 1;

			}

				if (shmctl(shm_id, IPC_RMID, NULL) < 0) {

				cerr << "Error: Failed to delete shared memory segment." << endl;

				return 1;

			}

			cout << "Shared memory segment deleted." << endl;

		} else {

			cerr << "Error: Invalid mode specified." << endl;

			return 1;

		}



	return 0;

}