Project: HTTP Client Server

Due Date: 2/24/2019

Authors: Hunter Werenskjold and Carson Wilber

Description:
	We aim to create an HTTP server and client in the C programming language. The server will listen on a given port in the 60001 to 60099 domain and respond to client requests to the port. In particular, the client will requests html files and the server will respond with sending the html file or an error message that the file could not be found.

Included Files:
	- fileio.c
	- fileio.h
	- filetypes.c
	- filetypes.h
	- http.h
	- http-client.c (Client's main)
	- http-craft.c
	- http-craft.h
	- http-parse.c
	- http-server.c (Server's main)
	- http-util.c
	- http-util.h
	- logging.c
	- logging.h
	- settcptimeout.c
	- util.h
	- index.html
	- Profile.jpg
	- screenshot-index-html.png
	- screenshot-404.png
	- README (this file)

How to Use:

Compilation)

1. Navigate to the directory containing Makefile

2. Execute 'make' or 'make all' to build the objects, executables, and other relevant files, including a library that archives all compiled objects for inclusion in any testing structure or application via a shared object (.so).

Running)

1. After compilation, launch both applications. You can easily do this with two terminal windows and by typing the commands "./http-server.x" or "./http-client.x". 

	Note: Be sure to initialize the server first to give your client a target server socket.
	
	Note: The server can timeout without requests, so commands must be issued within 30 seconds each.
	
	Note: You may specify a port number as the first argument to either executable. The default is 60001.

2. On the client, you are able to begin making requests.

3a. On the client, type http requests to the server, for instance:
	a.) index.html
	b.) fake.html
	c.) Profile.jpg

	Note: Due to a small buffer (4096 bytes) and large image data (like Profile.jpg), requesting an image will clog the TCP connection and subsequent requests by the client will result in receiving a server response that continues to the next 0-byte ('\0') in the image data. Request the image last for testing purposes.

3b. Type 'exit' in the client to end the TCP connection to the server.

4. View the server terminal during or after execution for verbose logs about server activity, including INFO, ERROR, and WARN messages, where applicable.
