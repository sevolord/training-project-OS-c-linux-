/*

Написать программу, которая умеет корректно реагировать на сигнал остановки процесса — родительского и дочернего.

запрограммировать:

1.   Создать 3 функции-обработчики сигналов: реакцию на завершение главного процесса, реакцию на завершение дочернего процесса (для него самого и для его родителя).

2.   Сменить диспозицию сигналов SIGINT, SIGCHLD и SIGKILL, завязав их на соответствующие функции.

3.   Запустить дочерний процесс, вывести на экран PID родительского и дочернего процессов, поставить процессы на паузу — pause().

*/

#include <iostream>

#include <csignal>

#include <unistd.h>



// Обработчик сигнала для завершения основного процесса

void sigint_handler(int signal) {

    std::cout << "Main process terminated" << std::endl;

    exit(0);

}



// Обработчик сигнала для завершения дочернего процесса

void sigchld_handler(int signal) {

    std::cout << "Child process terminated" << std::endl;

}



int main() {

    // Устанавливаем обработчики сигналов

    signal(SIGINT, sigint_handler);

    signal(SIGCHLD, sigchld_handler);



    // Создаем дочерний процесс

    pid_t pid = fork();



    if (pid == 0) {

        // Это код дочернего процесса

        std::cout << "Child process: PID = " << getpid() << std::endl;

        pause();

    } else {

        // Это код родительского процесса

        std::cout << "Parent process: PID = " << getpid() << std::endl;

        pause();

    }



    return 0;

}