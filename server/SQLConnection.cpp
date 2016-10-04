/**
*******************************************************************************
* @file		SQLConnection.cpp
* @brief 	This file provides the implementation of SQLConnection class.
* @author		Yifeng He
* @date			Mar. 12, 2014, Version 1.0
*******************************************************************************
**/

#include <iostream>
#include "SQLConnection.h"
#include "mysql.h"

using namespace std;



/**
*******************************************************************************
* @brief		The constructor to set up a database connection
* @param 		serverIP (char*) -- IP address of the server
* @param 		userName (char*) -- user name for database access
* @param 		passWord (char*) -- password for database access
* @param 		dbName (char*) -- database name
* @return 	void
*******************************************************************************
*/ 
SQLConnection::SQLConnection(const char* serverIP, const char* userName, const char* passWord, const char* dbName)
{	
	mysql_init(&connection);
  if(mysql_real_connect(&connection, serverIP, userName, passWord, dbName, 0, NULL,0)) 
  	cout << "Connection to database was successful.\n";
  else {
  	cout << "Connection to database failed.\n"; 
  }    
}
      
 
 
/**
*******************************************************************************
* @brief		This function executes a sql query
* @param 		sqlStatement (char*) -- the sql query statement 
* @return 	void
*******************************************************************************
*/ 
void SQLConnection::executeSQL(const char* sqlStatement)
{
	int returnVal;
	returnVal = mysql_query(&connection, sqlStatement);
  if(returnVal == 0) 
		cout << "Inserted " << (unsigned long)mysql_affected_rows(&connection) 
			<< " rows into the table." << endl;
  else
		cout << "Insert error " << mysql_errno(&connection) << ": " 
			<< mysql_error(&connection) << endl;
}
 
 
 
/**
*******************************************************************************
* @brief		The destructor
* @param 		void 
* @return 	void
*******************************************************************************
*/ 
SQLConnection::~SQLConnection()
{
	mysql_close(&connection);
}



