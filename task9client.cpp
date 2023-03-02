#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
using namespace std;


/*
Клиент:

1.   Создать сокет системным вызовом socket(). Дескриптор, который вернет этот системный вызов, используется во всех остальных системных вызовах для работы с созданным сокетом. Выдать сообщение, что сокет создан.

2.   Объявить переменную типа struct sockaddr_un и заполнить ее: поле sun_family равно АF_UNIX; поле sun_path равно тому имени файла, который придуман в п. 2 серверной программы.

3.   Присоединиться к серверному сокету системным вызовом connect().

4.   Для передачи и приема сообщений организовать бесконечный цикл while(true). В этом цикле выполнить следующее.

–       Выдать приглашение для ввода строки сообщения.

–       Взять от пользователя строку функцией gets(). Если введена пустая строка, то выйти из цикла, закрыть сокет и закончить работу программы.

–       Для непустых строк-сообщений: сначала отправлять на сервер сообщение системным вызовом send(), потом принимать от сервера сообщение системным вызовом recv().

–       Принятое сообщение выводить на экран.
*/



const int BUFFER_SIZE = 1024;



int main() {

	int client_sock;

	struct sockaddr_un server_addr;

	char buffer[BUFFER_SIZE];



	// create socket

	client_sock = socket(AF_UNIX, SOCK_STREAM, 0);

	if (client_sock < 0) {

		cerr << "Error creating socket" << endl;

		exit(-1);

	}

	cout << "Socket created" << endl;



	// initialize server address

	server_addr.sun_family = AF_UNIX;

	strcpy(server_addr.sun_path, "./echo.server");



	// connect to server

	int res = connect(client_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));

	if (res < 0) {

		cerr << "Error connecting to server" << endl;

		exit(-1);

	}

	cout << "Connected to server" << endl;



	while (true) {

		// prompt user for message

		cout << "Enter message: ";

		fgets(buffer, BUFFER_SIZE, stdin);

		buffer[strcspn(buffer, "\n")] = '\0'; // remove newline from input

		if (strlen(buffer) == 0) {

		  // exit if empty message received

		  break;

		}



		// send message to server

		//cout << "You message: " << buffer <<endl;

		send(client_sock, buffer, strlen(buffer), 0);



		// receive message from server

		int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);

		if (bytes_received < 0) {

		  cerr << "Error receiving message from server" << endl;

		  break;

		}

		buffer[bytes_received] = '\0'; // null-terminate received message



		// output message from server

		cout << "Message from server: " << buffer << endl;

	}



	close(client_sock);

	return 0;

}

