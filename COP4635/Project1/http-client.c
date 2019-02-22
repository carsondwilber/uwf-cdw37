// TCP Client
// 1. Create a client socket.
// 2. Connect to server socket.
// 3. Receive data.
// 4. Display data.
// 5. Close socket.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "util.h"
#include "http.h"

void handleRequests(int tcp_client_socket)
{
	char userResourceRequest[64] = {0};
	
	printf("Request: ");
	
	scanf("%63s", userResourceRequest);
	
	char *tcp_server_response = (char *)calloc(4096, sizeof(char));
	
	while (strcicmp(userResourceRequest, "exit") != 0)
	{
		char buffer[128];
		
		snprintf(buffer, 128, "GET %s HTTP/1.1\r\nAccept-Language: en\r\nConnection: keep-alive\r\n\r\n", userResourceRequest);
		
		send(tcp_client_socket, buffer, 128, 0);
		
		recv(tcp_client_socket, tcp_server_response, 4096, 0);
		
		printf("Received response of %ld bytes.\n", strlen(tcp_server_response));
		
		printf("%s\n", tcp_server_response);
		
		free(tcp_server_response);
		
		tcp_server_response = (char *)calloc(4096, sizeof(char));
		
		printf("Next request: ");
		
		scanf("%63s", userResourceRequest);
	}
	
	printf("Closing connection!\n");
	
	close(tcp_client_socket);
}

int main()
{
	// 1. Create a client socket.
	int tcp_client_socket;						// Socket descriptor
	tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);		// Open TCP socket.
	
	// 2-a. Define the server IP and port.
	struct sockaddr_in tcp_server_address;				// Address structure
	tcp_server_address.sin_family = AF_INET;			// Define address family
	tcp_server_address.sin_port = htons(39756);			// Define address port
	tcp_server_address.sin_addr.s_addr = INADDR_ANY;		// Connecting to 0.0.0.0
	
	// 2-b. Connect to the server.
	int connection_status =
		connect(
			tcp_client_socket,
			(struct sockaddr *) &tcp_server_address,
			sizeof(tcp_server_address)
		);
	
	if (connection_status == -1)
	{
		printf(" Problem connecting to the socket! Exiting.\n");
		
		return -1;
	}
	
	handleRequests(tcp_client_socket);
	
	printf("Done sending requests! Exiting now.\n");
	
	return 0;
}
