# Data-Transmission-Storage-and-Retrieval-using-PHP-and-MySQL
This project aims to develop a program enabling a server to collect the information from the client and then store it into MySQL database which can be retrieved by the other users via web browsers.

This program is dependent on boost and openCV libraries.

client.cpp

This client sends an image and the structured information to the server.

server.cpp

The server listens on a port and receives the data sent from the client, and then stores the structured information into MySQL database.

access_database.PHP

This PHP file is used to retrieve the information from the MySQL database and show it onto the web browser.



