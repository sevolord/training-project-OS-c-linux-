#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
using namespace std;

//===========================================================================


/*

Дополнить программу возможностью выполнения команд-конвейера с созданием программного канала. Программа должна уметь выполнять команды вида

команда1 [парам 1] … [парам N] | команда2 [парам 1] … [парам N].

Это означает, что команда1 направляет результаты своей работы команде2.

*/



const int MAX_COMMAND_LENGTH = 1024;

const int MAX_ARGS = 128;



void parseCommand(char* command, char** argv) {

  // Use strtok to parse the command into separate words

  char* token = strtok(command, " ");

  int i = 0;

  while (token != NULL) {

    argv[i] = token;

    i++;

    token = strtok(NULL, " ");

  }

  // Make sure the last element in the array is null

  argv[i] = NULL;

}





int main() {

  cout<< "Started myTerminal. Hello, Jedi" << endl;

  while (true) {

    char command[MAX_COMMAND_LENGTH];

   cout << "Enter command ";

   cin.getline(command, MAX_COMMAND_LENGTH);



    // Check if the user wants to exit the program

    if (strcmp(command, "by-by") == 0) {

      break;

    }



    char* argv[MAX_ARGS];

    parseCommand(command, argv);



    // проверяем, будет ли конвеер

    char* command2[MAX_ARGS];

    int pipefd[2];

    pid_t pid1, pid2;

    int status;

    bool conveyor = false;

    for (int i = 0; argv[i] != NULL; i++) {

      if (strcmp(argv[i], "|") == 0) {

        argv[i] = NULL;

        parseCommand(argv[i + 1], command2);

        conveyor = true;

        break;

      }

    }



    if (conveyor) { //если конвеер

      if (pipe(pipefd) == -1) {

        perror("Pipe error");

        exit(EXIT_FAILURE);

      }



      pid1 = fork();

      if (pid1 == 0) {

        // Child process 1

        // передаем вывод в программный канал

        dup2(pipefd[1], STDOUT_FILENO);

        close(pipefd[0]);

        close(pipefd[1]);

        if (execvp(argv[0], argv) == -1) {

          perror("Error executing command");

          exit(EXIT_FAILURE);

        }

      } else {

        pid2 = fork();

        if (pid2 == 0) {

          // Child process 2

          // передаем вывод в программный канал

          dup2(pipefd[0], STDIN_FILENO);

		  close(pipefd[0]);

          close(pipefd[1]);

          if (execvp(command2[0], command2) == -1) {

            perror("Error executing command");

            exit(EXIT_FAILURE);

          }

        } else {

          // Parent process

          close(pipefd[0]);

          close(pipefd[1]);

          // ждем дочерние процессы

          waitpid(pid1, &status, 0);

          waitpid(pid2, &status, 0);

        }

      }

    } else { 

      // Create a child process to execute the command

      pid_t pid = fork();

      if (pid == 0) {

        // Child process

        // Check if the command is "cd"

        if (strcmp(argv[0], "cd") == 0) {

          // Change the working directory

          if (argv[1] == NULL) {

            // If no directory is specified, go to the home directory

            char* home = getenv("HOME");

            if (chdir(home) != 0) {

              perror("Error changing directory");

              exit(EXIT_FAILURE);

            }

          } else {

            if (chdir(argv[1]) != 0) {

              perror("Error changing directory");

              exit(EXIT_FAILURE);

            }

          }

        }

        // Check if the command is "echo"

        else if (strcmp(argv[0], "echo") == 0) {

          // Output the string to the screen

          for (int i = 1; argv[i] != NULL; i++) {

           cout << argv[i] << " ";

          }

         cout <<endl;

        }

        // перенаправление вывода

        else {

          bool redirect = false;

          char* filename;

          for (int i = 0; argv[i] != NULL; i++) {

            if (strcmp(argv[i], ">") == 0) {

              redirect = true;

              filename = argv[i + 1];

              // Remove the redirect and filename from the argument list

              argv[i] = NULL;

              break;

            }

          }

          if (redirect) {

            // Redirect standard output to the file

            freopen(filename, "w", stdout);

          }

          // Execute the command

          if (execvp(argv[0], argv) == -1) {

            perror("Error executing command");

            exit(EXIT_FAILURE);

          }

        }

      } else {

        // Parent process

        // ждем дочерние процессы

        waitpid(pid, &status, 0);

      }

    }

  }

  return 0;

}

