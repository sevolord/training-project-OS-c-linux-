/*

    Программа работает в 5 разных режимах, в зависимости от переменной, 

	переданной с консоли: 

	(с) создание очереди, ./a.out c key

	(s) запись сообщения в очередь, ./a.out s key 1

	(r) чтение сообщения из очереди, ./a.out r key 1

	(р) чтение сообщения без его удаления (по факту чтение и запись),  ./a.out p key 1

	(d) удаление очереди. ./a.out d key

	

*/



#include <iostream>

#include <cstring>

#include <sys/ipc.h>

#include <sys/msg.h>



const int MAX_MESSAGE_LENGTH = 256;



struct Message {

long type;

char text[MAX_MESSAGE_LENGTH];

};



int main(int argc, char* argv[]) {

	if (argc < 2) {

		std::cerr << "Error: Invalid number of arguments. Mode not specified." << std::endl;

		return 1;

	}

	char mode = argv[1][0];

	key_t key;

	int queueId;



	switch (mode) {

		case 'c': {

			// Mode: Create queue

			if (argc < 3) {

				std::cerr << "Error: Invalid number of arguments. Key not specified." << std::endl;

				return 1;

			}

			key = std::stoi(argv[2]);

			queueId = msgget(key, IPC_CREAT | IPC_EXCL | 0666);

			if (queueId < 0) {

				std::cerr << "Error: Failed to create message queue." << std::endl;

				return 1;

			}

			std::cout << "Message queue created with key " << key << " and id " << queueId << std::endl;

			break;

		}

		case 's': {

			// Mode: Write message to queue

			if (argc < 4) {

				std::cerr << "Error: Invalid number of arguments. Key or type not specified." << std::endl;

				return 1;

			}

			key = std::stoi(argv[2]);

			queueId = msgget(key, 0666);

			if (queueId < 0) {

				std::cerr << "Error: Failed to access message queue." << std::endl;

				return 1;

			}

			//сначала вызываем, потом берем вводимые строки и сохраняем

			Message message;

			message.type = std::stoi(argv[3]);

			std::string input;

			std::string messageText;

			while (true) {

				std::getline(std::cin, input);

				if (input.empty()) { //пустая строка - выход

					break;

				}

				messageText += input + '\n';

			}

			std::strcpy(message.text, messageText.c_str());

			if (msgsnd(queueId, &message, messageText.length() + 1, IPC_NOWAIT) < 0) {

				std::cerr << "Error: Failed to send message to queue." << std::endl;

				return 1;

			}

			std::cout << "Message sent to queue with id " << queueId << " and type " << message.type << std::endl;

			break;

		}

		case 'r': {

			// Mode: Read message from queue

			if (argc < 4) {

				std::cerr << "Error: Invalid number of arguments. Key or type not specified." << std::endl;

				return 1;

			}

			key = std::stoi(argv[2]);       

			queueId = msgget(key, 0666);

			if (queueId < 0) {

				std::cerr << "Error: Failed to access message queue." << std::endl;

				return 1;

			}

			Message message;

			message.type = std::stoi(argv[3]);

			if (msgrcv(queueId, &message, MAX_MESSAGE_LENGTH, message.type, IPC_NOWAIT | MSG_NOERROR) < 0) {

				std::cerr << "Error: Failed to receive message from queue." << std::endl;

				return 1;

			}

			std::cout << "Message received from queue with id " << queueId << " and type " << message.type << ": " << std::endl;

			std::cout << message.text << std::endl;

			break;

		}

		case 'p': {

			// Mode: Read message without deleting

			if (argc < 4) {

				std::cerr << "Error: Invalid number of arguments. Key or type not specified." << std::endl;

				return 1;

			}

			key = std::stoi(argv[2]);

			queueId = msgget(key, 0666);

			if (queueId < 0) {

				std::cerr << "Error: Failed to access message queue." << std::endl;

				return 1;

			}

			Message message;

			message.type = std::stoi(argv[3]);

			if (msgrcv(queueId, &message, MAX_MESSAGE_LENGTH, message.type, IPC_NOWAIT | MSG_NOERROR) < 0) {

				std::cerr << "Error: Failed to receive message from queue." << std::endl;

				return 1;

			}

			std::cout << "Message received from queue with id " << queueId << " and type " << message.type << ": " << std::endl;

			std::cout << message.text << std::endl;

			if (msgsnd(queueId, &message, MAX_MESSAGE_LENGTH, IPC_NOWAIT) < 0) {

				std::cerr << "Error: Failed to send message back to queue." << std::endl;

				return 1;

			}

			break;

		}

		case 'd': {

			// Mode: Delete queue

			if (argc < 3) {

				std::cerr << "Error: Invalid number of arguments. Key not specified." << std::endl;

			return 1;

			}

			key = std::stoi(argv[2]);

			queueId = msgget(key, 0666);

			if (queueId < 0) {

				std::cerr << "Error: Failed to access message queue." << std::endl;

			return 1;

			}

			if (msgctl(queueId, IPC_RMID, nullptr) < 0) {

				std::cerr << "Error: Failed to delete message queue." << std::endl;

				return 1;

			}

			std::cout << "Message queue with id " << queueId << " deleted" << std::endl;

			break;

		}

		default: {

			std::cerr << "Error: Invalid mode specified." << std::endl;

			return 1;

		}

	}

	return 0;

}

