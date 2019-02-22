#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#include "util.h"
#include "http.h"
#include "http-parse.h"
#include "http-craft.h"
#include "logging.h"

int readFileIntoBuffer(char *filePath, char **fileData)
{
	FILE *fp = fopen(filePath, "r");
	
	if (fp != NULL)
	{
		/* Go to the end of the file. */
		if (fseek(fp, 0L, SEEK_END) == 0)
		{
			/* Get the size of the file. */
			long bufsize = ftell(fp);

			if (bufsize == -1)
			{
				log_msg(ERROR, "Error reading file!");
				
				fclose(fp);
				
				return -1;
			}

			/* Allocate our buffer to that size. */
			*fileData = malloc(sizeof(char) * (bufsize + 1));

			/* Go back to the start of the file. */
			if (fseek(fp, 0L, SEEK_SET) != 0)
			{
				log_msg(ERROR, "Error reading file!");
				
				free(*fileData);
				
				fclose(fp);
				
				return -1;
			}

			/* Read the entire file into memory. */
			size_t newLen = fread(*fileData, sizeof(char), bufsize, fp);
			
			if (ferror(fp) != 0)
			{
				log_msg(ERROR, "Error reading file!");
				
				free(*fileData);
				
				fclose(fp);
				
				return -1;
			}
			else
			{
				(*fileData)[newLen++] = '\0'; /* Just to be safe. */
			}
			
			return newLen;
		}
		
		log_msg(ERROR, "Error reading file!");
		
		fclose(fp);
		
		return -1;
	}
	
	log_msg(ERROR, "Error opening file!");
	
	fclose(fp);
	
	return -1;
}

void handleRequests(int tcp_client_socket)
{
	settcptimeout(tcp_client_socket, 30);
	
	char *tcp_client_message = (char *)malloc(sizeof(char) * 512);
	
	int recvBytes;
	
	while ((recvBytes = recv(tcp_client_socket, tcp_client_message, 512, 0)) != -1)
	{
		if (recvBytes == 0)
		{
			log_msg(INFO, "Received a packet with empty content; likely a FIN or ACK. Disconnecting.");
			
			free(tcp_client_message);
			
			break;
		}
		
		log_msg(INFO, "Received the following raw request:");
		
		printf("\n%s", tcp_client_message);
		
		char *requestType, *resource, *httpVersion;
		
		int requestHeaderParseResult = parseHTTPRequestHeader(tcp_client_message, &requestType, &resource, &httpVersion);
		
		if (requestHeaderParseResult != VALID_HEADER)
		{
			log_msg(ERROR, "Request is not well formed: returning 400 Bad Request.");
			
			char responseBuffer[128];
			
			int bytes = craftHTTPResponseHeader(DEFAULT_VERSION, HTTP_ERROR_400_BAD_REQUEST, responseBuffer, 128);
			
			bytes += craftHTTPField("Host", "localhost", responseBuffer + bytes, 128 - bytes);
			
			bytes += craftHTTPField("Connection", "Keep-Alive", responseBuffer + bytes, 128 - bytes);
			
			send(tcp_client_socket, responseBuffer, strlen(responseBuffer), 0);
			
			free(tcp_client_message);
			
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		{
			struct stat s;
			
			if (stat(resource, &s) != 0)
			{
				log_msg(ERROR, "Resource requested not found: returning 404 Not Found.");
				
				char responseBuffer[128];
				
				int bytes = craftHTTPResponseHeader(DEFAULT_VERSION, HTTP_ERROR_404_NOT_FOUND, responseBuffer, 128);
				
				bytes += craftHTTPField("Host", "localhost", responseBuffer + bytes, 128 - bytes);
				
				bytes += craftHTTPField("Connection", "Keep-Alive", responseBuffer + bytes, 128 - bytes);
				
				send(tcp_client_socket, responseBuffer, strlen(responseBuffer), 0);
				
				free(tcp_client_message);
				
				tcp_client_message = (char *)malloc(sizeof(char) * 512);
				
				continue;
			}
		}
		
		char *fileData;
		
		int readSize = readFileIntoBuffer(resource, &fileData);
		
		if (readSize == -1)
		{
			log_msg(ERROR, "Failed to read file: returning 500 Internal Server Error.");
			
			char responseBuffer[128];
			
			int bytes = craftHTTPResponseHeader(DEFAULT_VERSION, HTTP_ERROR_500_INTERNAL_SERVER_ERROR, responseBuffer, 128);
			
			bytes += craftHTTPField("Host", "localhost", responseBuffer + bytes, 128 - bytes);
			
			bytes += craftHTTPField("Connection", "Keep-Alive", responseBuffer + bytes, 128 - bytes);
			
			send(tcp_client_socket, responseBuffer, strlen(responseBuffer), 0);
			
			free(tcp_client_message);
			
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		char headersBuffer[128] = {0};
		
		log_msg(INFO, "Serving requested resource.");
		
		int bytes = craftHTTPResponseHeader(DEFAULT_VERSION, HTTP_SUCCESS_200_OK, headersBuffer, 128);
		
		bytes += craftHTTPField("Host", "localhost", headersBuffer + bytes, 128 - bytes);
		
		bytes += craftHTTPField("Connection", "Keep-Alive", headersBuffer + bytes, 128 - bytes);
		
		bytes += craftHTTPField("Content-Type", "text/html", headersBuffer + bytes, 128 - bytes);
		
		snprintf(headersBuffer + bytes, 128 - bytes, "Content-Length: %d\r\n", readSize);
		
		int totalLen = strlen(headersBuffer) + readSize + 2;
		
		char *responseBuffer = (char *)malloc(sizeof(char) * (totalLen + 1));
		
		snprintf(responseBuffer, totalLen, "%s\r\n%s", headersBuffer, fileData);
		
		responseBuffer[totalLen] = '\0';
		
		send(tcp_client_socket, responseBuffer, strlen(responseBuffer), 0);
		
		free(responseBuffer);
		
		free(fileData);
		
		free(tcp_client_message);
		
		tcp_client_message = (char *)malloc(sizeof(char) * 512);
	}
	
	log_msg(INFO, "Closing client socket.");
	
	close(tcp_client_socket);
}

int main()
{
	// 1. Create the server socket.
	
	log_msg(INFO, "Creating server socket.");
	
	int tcp_server_socket;					// Socket descriptor
	tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0);	// Open TCP socket
	
	// 2-a. Define the server IP and port.
	
	log_msg(INFO, "Defining server IP and port.");
	
	struct sockaddr_in tcp_server_address;			// Address structure
	tcp_server_address.sin_family = AF_INET;		// Define address family
	tcp_server_address.sin_port = htons(39756);		// Define address port
	tcp_server_address.sin_addr.s_addr = INADDR_ANY;	// Connecting to 0.0.0.0
	
	// 2-b. Bind the server IP and port.
	
	log_msg(INFO, "Binding server IP and port.");
	
	bind(
		tcp_server_socket,
		(struct sockaddr *) &tcp_server_address,
		sizeof(tcp_server_address)
	);
	
	// 3. Listen for incoming connection.
	
	log_msg(INFO, "Listening for client..");
	
	listen(tcp_server_socket, 5);
	
	// 4. Accept a new connection.
	
	int tcp_client_socket;
	tcp_client_socket = accept(tcp_server_socket, NULL, NULL);
	
	log_msg(INFO, "Found request: opening connection.");

	handleRequests(tcp_client_socket);
	
	log_msg(INFO, "Done accepting connections! Exiting now.");
	
	return 0;
}
