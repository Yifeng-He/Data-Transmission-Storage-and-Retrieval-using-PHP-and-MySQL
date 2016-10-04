/**
*******************************************************************************
* @file		SQLConnection.h
* @brief	This file provides the interfaces for database access
* @author		Yifeng He
* @date			Mar. 12, 2014, Version 1.0
*******************************************************************************
**/

#ifndef _SQLCONNECTION_H_
#define _SQLCONNECTION_H_

#include "mysql.h"

using namespace std;

/**
*******************************************************************************
* @class	SQLConnection
* @brief 	This class provides the interfaces for database access
*******************************************************************************
*/
class SQLConnection
{
  public:
  	//constructor
  	SQLConnection(const char* serverIP, const char* userName, const char* passWord, const char* dbName);
  	//execute a SQL qyuery
  	void executeSQL(const char* sqlStatement);
  	//deconstructor
  	~SQLConnection();

	private:
		MYSQL connection;
};

#endif //_SQLCONNECTION_H_



