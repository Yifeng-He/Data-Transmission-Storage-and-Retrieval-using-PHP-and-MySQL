/**
*******************************************************************************
* @file			server.cpp
* @brief 		This file is the server program which listens on a port, accepts and 
processes the requests from the clients.
* @author		Yifeng He
* @date			Mar. 12, 2014, Version 1.0
*******************************************************************************
**/

//c/c++ standard headers
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

//boost headers
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 

//openCV headers
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

//MySQL connection headers
#include "SQLConnection.h"
#include "mysql.h" //for access to MySQL

//namespaces used in this file
using namespace std;
using namespace cv;

//define UserInfo Data Type
typedef boost::tuple <
long, /*userID*/
std::string, /*userName*/
std::string, /*phoneNumber*/
std::string, /*email*/
int, /*age*/
double, /*latitude*/
double, /*longitude*/
long /*startTime*/ > UserInfo;

//the server port
unsigned short tcp_port = 1234;



/**
*******************************************************************************
* @brief		This function is used to handle the communications betwen the server 
and an accepted client. It is executed by the new thread created by the main thread.
* @param		boost::shared_ptr<boost::asio::ip::tcp::socket> socket -- a pointer 
to the socket between the server and the client
* @return		void
*******************************************************************************
*/
void session(boost::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
	std::cout << "[" << boost::this_thread::get_id() << "] starts to process request " << std::endl;
	boost::system::error_code error;
	
	//step 1):receive the stream data
	long userID;
	std::string userName;
	std::string phoneNumber;
	std::string email;
	int age;
	double latitude;
	double longitude;
	long startTime;
	
	int bytesStreamDataReceived = 0;
	char bufStreamData[8];
	boost::asio::streambuf request_buf;
	std::cout << "Start to receive stream data." << std::endl;
	bytesStreamDataReceived = boost::asio::read_until(*socket, request_buf, "\n\n");
	std::cout<< "received " << bytesStreamDataReceived << " bytes." << "\n";
	std::istream request_stream(&request_buf);
	//put the stream data into userInfo
	request_stream >> userID;
	request_stream.read(bufStreamData, 1); // eat the "\n"
	char name[128];
	request_stream.getline(name, 256);
	userName = name;
	request_stream >> phoneNumber;
	request_stream >> email;
	request_stream >> age;
	request_stream >> latitude;
	request_stream >> longitude;
	request_stream >> startTime;
	request_stream.read(bufStreamData, 2); // eat the "\n\n"
	//make the tuple
	UserInfo userInfo(userID, userName, phoneNumber, email, age, latitude, longitude, startTime); 
	//display the userInfo
	std::cout << "The received user info is as follows." << std::endl;
	std::cout << "userID: " << userInfo.get<0>() << std::endl;
	std::cout << "userName: " << userInfo.get<1>() << std::endl;
	std::cout << "phoneNumber: " << userInfo.get<2>() << std::endl;
	std::cout << "email: " << userInfo.get<3>() << std::endl;
	std::cout << "age: " << userInfo.get<4>() << std::endl;
	std::cout << "latitude: " << userInfo.get<5>() << std::endl;
	std::cout << "longitude: " << userInfo.get<6>() << std::endl;
	std::cout << "startTime: " << userInfo.get<7>() << std::endl;
	
	//step 2): receive the raw data
	char bufRawData[1024*120];
	std::cout << "Start to receive raw data." << std::endl;
	int bytesRawDataReceived = 0;
	bytesRawDataReceived = boost::asio::read(*socket, boost::asio::buffer(bufRawData), error);
	std::cout<< "received " << bytesRawDataReceived << " bytes." << "\n";
  
  //step 3): disconnect the socket
	socket->shutdown( boost::asio::ip::tcp::socket::shutdown_both, error);
	socket->close(error);
	
	//step 4): store the stream data into mySQL database
	//create the instance of database connection 
	SQLConnection myConnection("127.0.0.1", "yhe", "222222", "augmented_reality");
  stringstream strStream; 
  string line;
  //a sql query
  string sqlSatement;	
	//sql query statement
	strStream << "insert into userInfo (userID, userName, phoneNumber, email, age, latitude, longitude, startTime) values (" << userInfo.get<0>() << ", \"" << userInfo.get<1>() << "\", \"" << userInfo.get<2>() << "\", \"" << userInfo.get<3>() << "\", " << userInfo.get<4>() << ", " << userInfo.get<5>() << ", " << userInfo.get<6>() << ", " << userInfo.get<7>() << ");";
	//get a string from a stringstream
	sqlSatement = strStream.str();
	cout << "Executing SQL query: " << sqlSatement.c_str() << endl; //debug
	myConnection.executeSQL(sqlSatement.c_str());
	//clear the content
	strStream.str(""); 
	strStream.clear(); 
	sqlSatement.clear();
	
	//step 5): display the image
	Mat jpegimage = imdecode(Mat(1, bytesRawDataReceived, CV_8UC1, bufRawData),CV_LOAD_IMAGE_COLOR);
	boost::this_thread::sleep( boost::posix_time::milliseconds( 10000 ) );
	//imshow("Jpeg Image",jpegimage); 
	//waitKey(10);
}



/**
*******************************************************************************
* @brief		This is the main function which is the entrance of the server program.
* @param		int -- argc (the number of the arguments)
* @param		char* -- argv[] (the array of the argument strings)
* @return		int
*******************************************************************************
*/
int main(int argc, char* argv[])
{
	std::cout <<argv[0] << " listen on port " << tcp_port << std::endl;
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor acceptor(io_service, 
				boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), tcp_port));
	boost::system::error_code error;
	
	//infinitely listen on the port and accept the request from the client
	for(;;) {
		boost::shared_ptr<boost::asio::ip::tcp::socket> socket(new boost::asio::ip::tcp::socket(io_service));
		//boost::asio::ip::tcp::socket socket(io_service);
		acceptor.accept(*socket);
		std::cout << "get new client connection." << std::endl;
		//start a new thread to process the request
		boost::thread t(boost::bind(session, socket));
	}
	
	return 0;
}


