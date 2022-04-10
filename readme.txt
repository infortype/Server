Version: 1.0
Date: 06/11/2021
Language: C++ (Qt)
Standar C++: 20
Creator: Omar Segura Arévalo

Files: server.cpp - server.h

Instructions: 
1) Add a your project the .cpp and the .h files (copy in the directory).
2) Go to the file .pro and in add this line or add the arguments "QT +=sql".
3) Go to the main file and put in the top '#include "server.h"'. 
4) Create a object of the class server like this: "Server *server = new Server("nameOfTheSqliteFile.sqlite");" (remember delete de pointer).
5) Now call the fuction "create" through the pointer: "server->ConnectToBase();".
6) Create a object of the class table like this: "Table *table = new Table("nameOfTheTable");" (remember delete de pointer).
5) Now call the fuction "addRow" through the pointer: "table->AddRow("nameOfTheRow", typeOfTheRow, "additional info");". (if do you want more rows call the functions again).
6) Now call the fuction "create" through the pointer: "table->Create();".

You can add data, show data, update data etc. You can see this functions in server.h file. 


