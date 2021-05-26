#include <iostream>
#include <sqlite3.h>
#include "sqlite3.h"
#include <string>
#include "createtable.h"
#include <sstream>
#include <Windows.h>
#include<limits>
using namespace std;







int main(int argc, char** argv)
{
	sqlite3* DB;
	int exit;
	int choice; 
	int option = 1;
	int optionLogin = 1; //for login loop
	char* messaggeError;
	char* zErrMsg;
	sqlite3_stmt * stmt;
	int numberOfRows=0;
	int next_choice=1;
	int deletelimit;
	string user, pass;






	// creating database with username and password -> login (asuming that we have to create the login database-that's why i will use createTableLogin and insert funcs.
	sqlite3* DBLogin;

	exit = sqlite3_open("login.db", &DBLogin);
	if (exit)
		std::cerr << "Error open DBLogin " << sqlite3_errmsg(DBLogin) << std::endl;
	else
		std::cout << "Opened Login Database Successfully!" << std::endl;
	createTableLogin(DBLogin);
	//insertDbLogin(DBLogin); //- use this only once if you never launched this app and want 1 row of data.After first launch you can delete this line.
	



	//Login 
	cout << "Login\n";
	while (optionLogin)
	{
		cout << "username: ";
		cin >> user;
		cout << "password: ";
		cin >> pass;
		int check = CheckDb(DBLogin, user, pass);
		if (check)
		{
			system("cls"); // optional
			changeColor(3);
			cout << "Login Successful" << endl;
			changeColor(7);
			break;
		}
		else
		{
			cout << "Login failed-Wrong username/password" << endl;

		}
		
		cout << "Exit program ? write 0 to exit or any number !=0 to continue : ";
		cin >> optionLogin;
		if (optionLogin == 0)
			{
				system("PAUSE");
				return (0);
			}

		
	}
	//open DB
	exit = sqlite3_open("example.db", &DB);
	if (exit)
		std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
	else
		std::cout << "Opened Database Successfully!" << std::endl;

	/////////////////////////////////////////////////////////////////////
	numberOfRows = countDb(DB);
	deletelimit = 0; // in case that table doesn't have lines
	cout << "Current number of rows in database : " << numberOfRows<<"\n\n"<< endl;
	cout << "\n1 - create database\n2 - insert default data (4 rows) in database\n3 - insert newline in database\n4 - delete last line database\n5 - check how many rows you have in database\n6 - insert custom data in database \nSelect option : \n" << endl;
	while (option)
	{
		numberOfRows = countDb(DB);
		if (numberOfRows)
		{
			next_choice = 2;
			deletelimit = numberOfRows; // you can now delete lines from you db
		}

		cin >> choice;

		if (choice == 1)
		{
			createTable(DB);
			next_choice = 2;
		}

		if (choice == 2 && next_choice != 1)
		{
			insertDb(DB);
			numberOfRows = countDb(DB);
			deletelimit++;

		}
		else if (choice == 2 && next_choice == 1)
		{

			cout << "No Table to insert in - create table first" << endl;

		}

		if (choice == 3)
		{
			insertLineDb(DB, numberOfRows + 1, "Bon", "Jovi", "38", "YASSY", "8989.9");
			numberOfRows++;
			deletelimit++;
		}

		if (choice == 4 && next_choice != 1 && deletelimit != 0)
		{
			deleteLineDb(DB);
		}
		else if (choice == 4 && deletelimit == 0)
		{
			cout << "No Table for delete action - create table first\nNo lines left" << endl;
		}

		if (choice == 5) // show how many rows exists
		{
			int lineCounter = countDb(DB);
			cout<< "Current number of rows in database : " << lineCounter << "\n\n" << endl;
		}

		if (choice == 6) // insert custom row , no custom id
		{
			string age_6;
			string salary_6;
			string surname_6, name_6, adress_6;
			int flag; //to enter/exit loop - verify if input is ok
			int flag2 = 0; // for second while loop (age)
			int rowc = countDb(DB); // rowc=rowcounter , created new var. just to be sure that numberOfRows isn't modified by this choice or smth.
			cout << "name: ";
			cin >> name_6;
			flag = 0; //to enter/exit loop - verify if input is ok
			while (flag == 0)
			{
				if (startswith(name_6, "-") == TRUE)
				{
					flag2 = 0;
					cout << "Error\nType again(Do not start with -): ";

					while (flag2 == 0)  // keep writing until number starts with smth > 0
					{

						cin >> age_6;
						if (startswith(name_6, "-") == FALSE)
						{
							flag2 = 1;
						}
						else
						{
							cout << "Error\nType again(Only numbers.Do not start with 0): ";
						}
					}
				}
				if (name_6.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-") != std::string::npos)
				{
					cout << "Error\nType again(Only alphabetical characters): ";
					cin >> name_6;
				}
				else
					flag = 1;
			}

			flag = 0;
			cout << "surname: ";
			cin.ignore();          //for getline 
			getline(cin, surname_6);
			while (flag == 0)
			{
				if (startswith(surname_6, "-") == TRUE)
				{
					flag2 = 0;
					cout << "Error\nType again(Do not start with -): ";

					while (flag2 == 0)  // keep writing until number starts with smth > 0
					{

						getline(cin, surname_6);
						if (startswith(surname_6, "-") == FALSE)
						{
							flag2 = 1;
						}
						else
						{
							cout << "Error\nType again(Only numbers.Do not start with 0): ";
						}
					}

				}
				if (surname_6.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ- ") != std::string::npos)
				{
					cout << "Error\nType again(Only alphabetical characters): ";
					cin >> surname_6;
				}
				else
					flag = 1;
			}

			cout << "age between 14 and 65: ";
			cin >> age_6;
		    flag = 0; 
			
			while (flag == 0)
			{

				if (startswith(age_6, "0") == TRUE )
				{
					flag2 = 0; 
					cout << "Error\nType again(Only numbers.Do not start with 0): ";
					
					while (flag2 == 0)  // keep writing until number starts with smth > 0
					{
						
						cin >> age_6;
						if (startswith(age_6, "0") == FALSE)
						{
							flag2 = 1;
						}
						else
						{
							cout << "Error\nType again(Only numbers.Do not start with 0): ";
						}
					}
					
				}

				if (age_6<"14" || age_6>"65")
				{
					flag2 = 0;
					cout << "Error\nType again(Only numbers.Do not start with 0.Age between 14 and 65): ";

					while (flag2 == 0)  // keep writing until number <=65 or >=14
					{

						cin >> age_6;
						if (age_6>="14" && age_6<="65")
						{
							flag2 = 1; 
						}
						else
						{
							cout << "Error\nType again(Only numbers.Do not start with 0)Age between 14 and 65): ";
						}
					}


				}
				

				

				if (age_6.find_first_not_of("1234567890") != std::string::npos )
				{
					cout << "Error\nType again(Only numbers.Do not start with 0): ";
					cin >> age_6;
					
				}
				else
					flag = 1;
			}
		
			



			
			cout << "adress: ";
			flag = 0;
			cin.ignore();          //for getline 
			getline (cin,adress_6);

			while (flag == 0)
			{
				if (startswith(adress_6, "0") == TRUE || startswith(adress_6, "1") == TRUE || startswith(adress_6, "2") == TRUE || startswith(adress_6, "3") == TRUE || startswith(adress_6, "4") == TRUE || startswith(adress_6, "5") == TRUE || startswith(adress_6, "6") == TRUE || startswith(adress_6, "7") == TRUE || startswith(adress_6, "8") == TRUE || startswith(adress_6, "9") == TRUE)
				{
					flag2 = 0;
					cout << "Error\nType again(Do not start with numbers): ";

					while (flag2 == 0)  // keep writing until number starts with smth > 0
					{

						getline(cin, adress_6);
						if (startswith(adress_6, "0") == FALSE || startswith(adress_6, "1") == FALSE || startswith(adress_6, "2") == FALSE || startswith(adress_6, "3") == FALSE || startswith(adress_6, "4") == FALSE || startswith(adress_6, "5") == FALSE || startswith(adress_6, "6") == FALSE || startswith(adress_6, "7") == FALSE || startswith(adress_6, "8") == FALSE || startswith(adress_6, "9") == FALSE)
						{
							flag2 = 1;
						}
						else
						{
							cout << "Error\nType again(Only numbers.Do not start with 0): ";
						}
					}
					
				}

				if (adress_6.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. ") != std::string::npos)
				{
					cout << "Error\nType again(only letters,numbers and (.): ";
					getline(cin, adress_6);
				}
				else
					flag = 1;
			}
			
			cout << "salary: ";
			cin >> salary_6;
			flag = 0;
			while (flag == 0)
			{
				if (salary_6.find_first_not_of("0123456789.") != std::string::npos)
				{
					cout << "Error\nType again: ";
					cin >> salary_6;
				}
				else
					flag = 1;
			}

			
			insertLineDb(DB, rowc + 1, name_6, surname_6,age_6, adress_6, salary_6);
			

		}
		

		cout << "Exit program ? write 0 to exit or any number !=0 to continue : ";
		cin >> option;
		if (option == 0)
		{
			break;
		}
		cout << endl;
		cout << "\n1 - create database\n2 - insert default data (4 rows) in database\n3 - insert newline in database\n4 - delete last line database\n5 - check how many rows you have in database\n6 - insert custom data in database \nSelect option : \n" << endl;
	}
// Columns to text	 
/*	char sSql[1000];
	sprintf_s(sSql, "select USERNAME , PASSWORD from LOGIN");
	sqlite3_prepare_v2(DBLogin, sSql, -1, &pSTmt, 0);
	iReturnCode = sqlite3_step(pSTmt);
	int numberOfRowsLogin = countDbLogin(DBLogin);
	cout << numberOfRowsLogin << endl;

	while (numberOfRowsLogin)
	{
		switch (iReturnCode)
		{
		case SQLITE_DONE:
			cout << "Warning: No record is found!" << endl;
			bRet = false;
			break;

		case SQLITE_ROW:

			string sUser = (const char*)sqlite3_column_text(pSTmt, 0);
			string sPass = (const char*)sqlite3_column_text(pSTmt, 1);
			cout << sUser << " " << sPass << endl;
			bRet = true;
			break;

		}
		numberOfRowsLogin--;
		iReturnCode = sqlite3_step(pSTmt);
	}
	sqlite3_finalize(pSTmt);
*/


    //insertLineDb(DB,int id,string name,string surname,string age,string adress,string salary)
	//int countDb(DB);
	//createTable(DB);
	//insertDb(DB);



	
	sqlite3_close(DBLogin);
	sqlite3_close(DB);
	system("PAUSE");
	return (0);
}