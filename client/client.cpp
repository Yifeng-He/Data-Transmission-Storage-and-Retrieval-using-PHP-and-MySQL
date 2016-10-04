/**
*******************************************************************************
* @file			client.cpp
* @brief 		This file is the client program which connects to the server, and 
sends/receives data to/from the server.
* @author		Yifeng He
* @date			Mar. 12, 2014, Version 1.0
*******************************************************************************
**/

//c/c++ standard headers
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib> //for srand() and rand()
#include <ctime>

//boost headers
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp> 
#include <boost/tuple/tuple_io.hpp> 

//openCV headers
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>

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
double, /*logitude*/
long /*startTime*/ > UserInfo; 



/**
*******************************************************************************
* @brief		This is the main function which is the entrance of the client program.
* @param		int -- argc (the number of the arguments)
* @param		char* -- argv[] (the array of the argument strings)
* @return		int
*******************************************************************************
*/
int main(int argc, char* argv[])
{
	//step 1): prepare the stream data to be sent
	srand (time(NULL)); // initialize random seed
  UserInfo userInfo(rand() % 10000 + 1, "Tom Jackson", "4161259845", "TJackson@google.com",
  	29, 1234.56, 78954.13, time(0));
  boost::asio::streambuf request;
  std::ostream request_stream(&request);
  request_stream << userInfo.get<0>() << "\n" << userInfo.get<1>() << "\n"
  	<< userInfo.get<2>() << "\n" << userInfo.get<3>() << "\n" << userInfo.get<4>() << "\n"
  	<< userInfo.get<5>() << "\n" << userInfo.get<6>() << "\n" << userInfo.get<7>() << "\n\n";
  int lengthBuf = request.size();	

	//step 2): prepare the raw data to be sent
  Mat src = imread("lenna.png"); 
  //jpeg compression parameters
  vector<uchar> buff;//buffer to hold the encoded bits
  vector<int> param = vector<int>(2);
  param[0] = CV_IMWRITE_JPEG_QUALITY;
  param[1] = 95;//default(95) 0-100
  //JPEG compression
  imencode(".jpg",src,buff,param);
  int jpgSize = buff.size(); //in bytes
  cout<<"coded file size(jpg) "<< jpgSize << " bytes." << endl;//fit buff size automatically.

	//step 3): set up the connection to the server
  std::string serverIP = "127.0.0.1";
  int serverPort = 1234;
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::resolver resolver(io_service);
  boost::asio::ip::tcp::resolver::query query(serverIP, boost::lexical_cast<std::string>(serverPort));
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  boost::asio::ip::tcp::resolver::iterator end;
  boost::asio::ip::tcp::socket socket(io_service);
  boost::system::error_code error = boost::asio::error::host_not_found;
  //attempting to establish a connection to each endpoint until it succeeds or runs out of endpoints
  while (error && endpoint_iterator != end)
  {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
  }
  if (error) {
  	std::cout << "Failed to connect!" << std::endl;
  	return 1;
  }
  std::cout << "connected to " << serverIP << " : " << serverPort << std::endl;
    
	//step 4): send the stream data over the socket
	int bytesStreamDataSent;
	std::cout << "start sending the following stream data.\n";
	cout << userInfo.get<0>() << "\n" << userInfo.get<1>() << "\n"
		<< userInfo.get<2>() << "\n" << userInfo.get<3>() << "\n" << userInfo.get<4>() << "\n"
		<< userInfo.get<5>() << "\n" << userInfo.get<6>() << "\n" << userInfo.get<7>() << "\n\n";
  bytesStreamDataSent = boost::asio::write(socket, request);  
  std::cout << "Expected to send " << lengthBuf << ". Actually sent " 
  	<< bytesStreamDataSent << " bytes" << std::endl;

	//step 5): send the raw data
	int bytesRawDataSent;
  std::cout << "start sending the raw data.\n";  
  bytesRawDataSent = boost::asio::write(socket, boost::asio::buffer(&buff[0], jpgSize));
  std::cout << "Expected to send " << jpgSize << ". Actually sent " 
  	<< bytesRawDataSent << " bytes" << std::endl;
  
  //step 6): disconnect the socket
	//Disable sends or receives on the socket
	socket.shutdown( boost::asio::ip::tcp::socket::shutdown_both, error);
	//Close the socket. 
	socket.close(error);	
	
  return 0;
} 



