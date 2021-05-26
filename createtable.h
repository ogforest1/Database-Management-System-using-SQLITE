#pragma once
#include <iostream>
#include <sqlite3.h>
#include "sqlite3.h"
#include <string>
#include <sstream>
#include <windows.h>
#include <cstring>

using namespace std;

bool startswith(const std::string& str, const std::string& cmp)    //for strings , smth like s[0]="0"
{
	return str.compare(0, cmp.length(), cmp) == 0;
}

void changeColor(int desiredColor) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), desiredColor);
	//changeColor(7); // Set the color back to white.
	// changeColor(12); // Change the color to red. 
	// 3 for blue
	// 2 for green
}



void createTable(sqlite3* DB)
{
	char* messaggeError;
	int exit = 0;
	string sql2;
	sql2 = "CREATE TABLE PERSON("
		"ID INT PRIMARY KEY     NOT NULL, "
		"NAME           TEXT    NOT NULL, "
		"SURNAME        TEXT    NOT NULL, "
		"AGE            TEXT    NOT NULL, "
		"ADDRESS        CHAR(50), "
		"SALARY         TEXT NOT NULL);";
	exit = 0;
	exit = sqlite3_open("example.db", &DB);
	exit = sqlite3_exec(DB, sql2.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error Create Table(probably already created?)" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
	{
		changeColor(3);
		cout << "Table created Successfully" << std::endl;
		changeColor(7);
	}
}

void createTableLogin(sqlite3* DBLogin)
{
	char* messaggeError;
	int exit = 0;
	string sql2;
	sql2 = "CREATE TABLE LOGIN("
		"USERNAME          TEXT    NOT NULL, "
		"PASSWORD          TEXT     NOT NULL ); ";
	
	exit = 0;
	exit = sqlite3_open("login.db", &DBLogin);
	exit = sqlite3_exec(DBLogin, sql2.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		//std::cerr << "Error Create Table(probably already created?)" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
	{
		changeColor(3);
		cout << "Table created Successfully" << std::endl;
		changeColor(7);
	}
}

void insertDbLogin(sqlite3* DB)
{
	char* messaggeError;
	int exit;

	string sql(
		"INSERT INTO LOGIN VALUES('student', 'password');"); //modify this to add new rows of data

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		//std::cerr << "Error Insert-maybe no database created or duplicate ids" << std::endl;
		sqlite3_free(messaggeError);
	}
	


}







void insertDb(sqlite3* DB)
{
	char* messaggeError;
	int exit;
	
	string sql(
		"INSERT INTO PERSON VALUES(1, 'PAUL', 'JOBS', '24', 'SEATTLE', '9900.0');"
		"INSERT INTO PERSON VALUES(2, 'PAUL', 'JOBS', '25', 'SEATTLE', '9900.0');"
		"INSERT INTO PERSON VALUES(3, 'PAUL', 'JOBS', '26', 'SEATTLE', '9900.0');"
		"INSERT INTO PERSON VALUES(4, 'PAUL', 'JOBS', '27', 'SEATTLE', '9900.0');");
	
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error Insert-maybe no database created or duplicate ids" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
	{
		changeColor(3);
		cout << "Records created Successfully!" << endl;	
		changeColor(7);
	}


}


void insertLineDb(sqlite3* DB,int id,string name,string surname,string age,string adress,string salary)
{
	
	
	char* messaggeError;
	int exit;
	ostringstream oss;
	
		oss << "INSERT INTO PERSON VALUES(" << id << ",'" << name << "','" << surname << "'," << age << ",'" << adress << "'," << salary << ");";
	string var = oss.str();
	
	string sql(var);

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error InsertLine(maybe create database OR age/salary isn't a number or has a character near number)" << std::endl;
		sqlite3_free(messaggeError);
		
	}
	else
	{
		changeColor(3);
		cout << "Insert Successfully!" << endl;
		changeColor(7);
	}


}




extern "C" int callback2(void* data, int count, char** rows, char**)
{
	if (count == 1 && rows) {
		*static_cast<int*>(data) = atoi(rows[0]);
		return 0;
	}
	return 1;
}

int countDbLogin(sqlite3* DB)
{
	int rc;
	char* zErrMsg;
	int count = 0;
	const char* sql3 = "SELECT count(*) FROM LOGIN";
	rc = sqlite3_exec(DB, sql3, callback2, &count, &zErrMsg);
	return count;

}




int countDb(sqlite3* DB)
{
	int rc;
	char* zErrMsg;
	int count = 0;
	const char* sql3 = "SELECT COUNT(ID) FROM PERSON";
	rc = sqlite3_exec(DB, sql3, callback2, &count, &zErrMsg);
	return count;

}

static int callback(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}




int CheckDb(sqlite3* DBLogin, string user, string pass)  //1 - true 0 - false
{
	int iReturnCode = 0;
	
	sqlite3_stmt * pSTmt;
	char sSql[1000];
	sprintf_s(sSql, "select USERNAME , PASSWORD from LOGIN");
	sqlite3_prepare_v2(DBLogin, sSql, -1, &pSTmt, 0);
	iReturnCode = sqlite3_step(pSTmt);
	int numberOfRowsLogin = countDbLogin(DBLogin);
	int flagUser=0; // compare strings.
	int flagPass = 0; // compare strings.
	
	int counter = numberOfRowsLogin; //other rows not seen - fixed

	while (numberOfRowsLogin)
	{
		switch (iReturnCode)
		{
		case SQLITE_DONE:
			cout << "Warning: No record is found!" << endl;
			
			break;

		case SQLITE_ROW:

			string sUser = (const char*)sqlite3_column_text(pSTmt, 0);
			string sPass = (const char*)sqlite3_column_text(pSTmt, 1);
			//cout << sUser << " " << sPass << endl;
			
			int x = sUser.compare(user);
			int y = sPass.compare(pass);
			if (x==0)
			{
				flagUser = 1;
			}
			else
			{
				flagUser = 0;
			}

			if (y==0)
			{
				flagPass = 1;
			}
			else
			{
				flagPass = 0;
			}

			if (flagUser == 1 && flagPass == 1)
			{
			  
				return 1;
			}

			else if( flagUser ==0 || flagPass==0 )
			{
				counter--;

			}
			if (counter == 0)
			{
				return 0;
			}
	

			break;

		}
		numberOfRowsLogin--;
		
		iReturnCode = sqlite3_step(pSTmt); //for next row
	}
	
	sqlite3_finalize(pSTmt);

}





void deleteLineDb(sqlite3* DB)
{
	ostringstream oss;
	int numberOfRows;
	numberOfRows = countDb(DB);
	int exit;
	char* messaggeError;

	
	
	
		oss << "DELETE FROM person WHERE id=" << numberOfRows;
		string var = oss.str();

		string sql(var);

		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
		if (exit != SQLITE_OK || numberOfRows==0) {
			//std::cerr << "Error DeleteLine" << std::endl;
			cout << "Error DeleteLine | no lines left or error in sql statement" << endl;
			sqlite3_free(messaggeError);
		}
		else 
		{
			changeColor(3);
			cout << "Line deleted Successfully!" << endl;
			changeColor(7);
		}

	

}


