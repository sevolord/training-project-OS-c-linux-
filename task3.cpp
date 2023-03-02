//1.   Взять с командной строки имя файла.

//2.   Вызвать функцию stat(), передав ей имя файла и буфер для записи информации.

//3.   Для вывода на экран нужной информации использовать следующие поля структуры struct stat и макросы:

//1)   st_mode; /*режим доступа к файлу и информация о типе  файла*/;

//2)   st_uid; /*идентификатор пользователя*/;

//3)   st_gid; /*идентификатор группы*/;

//4)   st_size; /*размер файла в байтах*/;

//5)   st_mtime; /*время последней модификации содержимого файла*/;

//6)   макрос S_ISDIR(st_mode) возвращает true, если файл является каталогом;

//7)   макрос SISREG(st_mode) возвращает true, если файл является обычным файлом.

//4.   Для получения символьных имен пользователя и группы используйте вызовы getpwuid() и getgrgid():

//–       Объявить переменную буфер типа struct passwd, в которую записать возвращаемое значение getpwuid(). В поле *pw_name этой структуры будет //храниться входное имя пользователя.

//–       Объявить переменную буфер типа struct group, в которую записать возвращаемое значение getgrgid(). В поле *gr_name этой структуры будет храниться имя группы.

//5.   Время последней модификации файла привести в удобочитаемый вид функцией ctime().



#include <iostream>

#include <sys/types.h>

#include <sys/stat.h>

#include <unistd.h>

#include <pwd.h>

#include <grp.h>

#include <time.h>



using namespace std;



int main(int argc, char *argv[])

{

  if (argc != 2) // проверяем, что было передано имя файла

  {

    cerr << "Usage: " << argv[0] << " filename" << endl;

    return 1;

  }



  struct stat buf;// переменная для сохранения информации о файле



  if (stat(argv[1], &buf) == -1) // вызываем stat() с именем файла и адресом переменной buf

  {

    cerr << "Error: unable to get file information" << endl;

    return 1;

  }



  cout << "Information about " << argv[1] << ":" << endl;



  // file type

  if (S_ISDIR(buf.st_mode))

    cout << "Type: directory" << endl;

  else if (S_ISREG(buf.st_mode))

    cout << "Type: regular file" << endl;

  else

    cout << "Type: unknown" << endl;



  

  // получить имена пользователя и групп

  struct passwd *pw = getpwuid(buf.st_uid);

  struct group *gr = getgrgid(buf.st_gid);

  

  cout << "User: " << (pw != NULL ? pw->pw_name : "unknown") << endl;

  cout << "Group: " << (gr != NULL ? gr->gr_name : "unknown") << endl;



  //  размер файла и время последней модификации

  cout << "Size: " << buf.st_size << " bytes" << endl;

  cout << "Last modified: " << ctime(&buf.st_mtime);

  

  return 0;

}



