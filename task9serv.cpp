#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

using namespace std;


/*
Сервер:

1.   Создать сокет системным вызовом socket(). Дескриптор, который вернет этот системный вызов, используется во всех остальных системных вызовах для работы с созданным сокетом. Выдать сообщение, что сокет создан.

2.   Объявить переменную типа struct sockaddr_un для хранения адреса сервера. Придумать имя файла-адреса для сокета, например, "./echo.server". Перед связыванием сокета и файла этот файл (на случай, если он уже создан) удалить функцией unlink(). Смотри примеры в «Справочной информации» ниже.

3.   Заполнить переменную «адрес сервера»: поле sun_family равно АF_UNIX; поле sun_path равно тому имени файла, который придуман в п. 2.

4.   Связать сокет системным вызовом bind() с адресом. Выдать сообщение, что сокет связан.

5.   Перевести сокет в слушающий режим системным вызовом listen(). Выдать сообщение, что сокет слушает и готов принимать клиентов.

6.   Для приема клиентов организовать бесконечный цикл while(true). В этом цикле:

–       Блокировать родительский процесс системным вызовом accept() до прихода очередного клиента. Этот системный вызов разблокируется и вернет новый клиентский сокет, когда присоединится клиент. Все общение с конкретным клиентом осуществлять через его клиентский сокет.

–       Выдать сообщение, что пришел новый клиент.

–       Когда приходит клиент, создать для общения с ним дочерний процесс. В этом дочернем процессе: закрыть лишний серверный сокет; запустить бесконечный цикл приема-передачи сообщений while(true).

–       В этом бесконечном цикле принимать сообщение от клиента системным вызовом recv(), а потом отправлять это же сообщение обратно системным вызовом send(). Можно добавить к отправляемому сообщению слово “Еcho”.

–       Сообщение от клиента выдать на экран.

7.   Переписать диспозицию сигнала SIGINT в родительском и в дочерних процессах. В функциях-обработчиках сигналов закрыть соответствующие сокеты, выдать сообщение об окончании работы программы.

*/



const int BUFFER_SIZE = 1024;



void sigint_handler(int signum) {

	cout << "Server shutting down" << endl;

	unlink("./echo.server"); // unlink socket file

	exit(0);

}



int main() {

	int server_sock, client_sock;

	struct sockaddr_un server_addr, client_addr;

	char buffer[BUFFER_SIZE];



	// create socket

	server_sock = socket(AF_UNIX, SOCK_STREAM, 0);

	if (server_sock < 0) {

		cerr << "Error creating socket" << endl;

		exit(-1);

	}

	cout << "Socket created" << endl;



	// delete socket file if it already exists

	unlink("./echo.server");



	// initialize server address

	server_addr.sun_family = AF_UNIX;

	strcpy(server_addr.sun_path, "./echo.server");



	// bind socket to address

	int res = bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));

	if (res < 0) {

		cerr << "Error binding socket" << endl;

		exit(-1);

	}

	cout << "Socket bound" << endl;



	// start listening for clients

	listen(server_sock, 5);

	cout << "Listening for clients" << endl;



	// set up signal handler for graceful shutdown

	signal(SIGINT, sigint_handler);



	while (true) {

		socklen_t client_addr_len = sizeof(client_addr);

		// accept incoming client

		client_sock = accept(server_sock, (struct sockaddr*) &client_addr, &client_addr_len);

		if (client_sock < 0) {

		  cerr << "Error accepting client" << endl;

		  continue;

		}

		cout << "New client connected" << endl;



		// fork to handle communication with client in a separate process

		int pid = fork();

		if (pid == 0) {

		  // child process

		  close(server_sock); // close server socket, no longer needed

		  while (true) {

			// receive message from client

			int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);

			if (bytes_received < 0) {

			  cerr << "Error receiving message from client" << endl;

			  break;

			} else if (bytes_received == 0) {

			  cout << "Client disconnected" << endl;

			  break;

			}

			buffer[bytes_received] = '\0'; // null-terminate received message

			// prepend "echo" to message and send it back to client

			int outStrLen = BUFFER_SIZE +6 ;

			char echoStr[outStrLen] = "echo: ";

			strcat(echoStr, buffer); 

			send(client_sock, echoStr, outStrLen, 0);



			// output message from client to screen

			cout << "Message from client: " << buffer << endl;

			}

		  close(client_sock);

		  exit(0);

		} else {

		  // parent process

		  close(client_sock); // close client socket, no longer needed

		}

	}



	return 0;

}

