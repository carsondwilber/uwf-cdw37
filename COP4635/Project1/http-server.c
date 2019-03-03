/*
 * File: http-server.c
 * Created: 14 February 2019
 * Creators: Carson Wilber & Hunter Werenskjold
 * 	* Based on original source code provided by Dr. Amitabh Mishra
 * Purpose: The HTTP Server application.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdbool.h>

#include "util.h"
#include "http.h"
#include "http-parse.h"
#include "http-craft.h"
#include "logging.h"
#include "filetypes.h"
#include "fileio.h"
#include "http-util.h"

void sendAndConsume(int tcp_client_socket, char *data, int bytes)
{
	log_msg(INFO, "Sending response:");
	
	printf("\n%s\n\n", data);
	
	send(tcp_client_socket, data, bytes, 0);
	
	free(data);
}

char * FQAddress(char *address, int port)
{
	char *buffer = (char *)calloc(strlen(address) + 7, sizeof(char));
	
	snprintf(buffer, strlen(address) + 6, "%s:%d", address, port);
	
	return buffer;
}

void getAddress(int tcp_client_socket, char **ipstr, int *port)
{
	socklen_t len;
	struct sockaddr_storage addr;
	*ipstr = (char *)calloc(INET6_ADDRSTRLEN, sizeof(char));
	
	len = sizeof(addr);
	getpeername(tcp_client_socket, (struct sockaddr*)&addr, &len);

	if (addr.ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		*port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, *ipstr, sizeof(*ipstr));
	}
	else
	{ // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		*port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, *ipstr, sizeof(*ipstr));
	}
}

char path[PATH_MAX];

char * join(char *path1, char *path2)
{
	int len = strlen(path1) + strlen(path2) + 1;
	
	snprintf(path, len + 1, "%s/%s", path1, path2);
	
	path[len] = '\0';
	
	return path;
}

void handleRequests(int tcp_server_socket, int tcp_client_socket)
{
	settcptimeout(tcp_client_socket, 30);
	
	char *tcp_client_message = (char *)malloc(sizeof(char) * 512);
	
	char serverPath[PATH_MAX];
	getcwd(serverPath, sizeof(serverPath));
	
	int recvBytes;
	
	char *peerAddress;
	int peerPort;
	getAddress(tcp_client_socket, &peerAddress, &peerPort);
	
	bool disconnect = false;
	
	while ((recvBytes = recv(tcp_client_socket, tcp_client_message, 512, 0)) != -1)
	{
		if (recvBytes == 0)
		{
			if (disconnect)
			{
				log_msg(INFO, "Received a packet with empty content after a separate empty packet; likely a final ACK. Disconnecting.");
				
				free(tcp_client_message);
				
				break;
			}
			
			log_msg(INFO, "Received a packet with empty content; likely a FIN or ACK. Attempting second read.");
			
			disconnect = true;
			
			free(tcp_client_message);
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		disconnect = false;
		
		tcp_client_message[recvBytes] = '\0';
		
		log_msg(INFO, "Received the following raw request:");
		
		printf("\n%s", tcp_client_message);
		
		char *requestType, *resource, *httpVersion;
		
		int requestHeaderParseResult = parseHTTPRequestHeader(tcp_client_message, &requestType, &resource, &httpVersion);
		
		if (requestHeaderParseResult != VALID_HEADER)
		{
			if (requestHeaderParseResult == UNKNOWN_METHOD)
			{
				log_msg(ERROR, "Request uses an unsupported or unknown method! Returning 501 Not Implemented.");
				
				char *errorResponse = craftStandardErrorResponse(HTTP_ERROR_501_NOT_IMPLEMENTED);
				
				if (!errorResponse)
				{
					log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
					
					close(tcp_client_socket);
					
					break;
				}
				
				sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
			}
			else
			{
				log_msg(ERROR, "Request is not well formed: returning 400 Bad Request.");
				
				char *errorResponse = craftStandardErrorResponse(HTTP_ERROR_400_BAD_REQUEST);
				
				if (!errorResponse)
				{
					log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
					
					close(tcp_client_socket);
					
					break;
				}
				
				sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
			}
			
			free(tcp_client_message);
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		{
			struct stat s;
			
			if (stat(join(serverPath, resource), &s) != 0)
			{
				log_msg(ERROR, "Resource requested not found: returning 404 Not Found.");
				
				char * errorResponse = craftStandardErrorResponse(HTTP_ERROR_404_NOT_FOUND);
				
				if (!errorResponse)
				{
					log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
					
					close(tcp_client_socket);
					
					break;
				}
				
				sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
				
				free(tcp_client_message);
				tcp_client_message = (char *)malloc(sizeof(char) * 512);
				
				continue;
			}
		}
		
		char *fileData;
		
		int readSize = readFileIntoBuffer(join(serverPath, resource), &fileData);
		
		if (readSize == -1)
		{
			log_msg(ERROR, "Failed to read file: returning 500 Internal Server Error.");
			
			char * errorResponse = craftStandardErrorResponse(HTTP_ERROR_500_INTERNAL_SERVER_ERROR);
			
			if (!errorResponse)
			{
				log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
				
				close(tcp_client_socket);
				
				break;
			}
			
			sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
			
			free(tcp_client_message);
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		char *fileExtension = getFileExtension(resource);
		
		if (fileExtension == NULL)
		{
			log_msg(ERROR, "Request is not well formed: returning 400 Bad Request.");
			
			char *errorResponse = craftStandardErrorResponse(HTTP_ERROR_400_BAD_REQUEST);
			
			if (!errorResponse)
			{
				log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
				
				close(tcp_client_socket);
				
				break;
			}
			
			sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
			
			free(tcp_client_message);
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		int fileType = getFileType(fileExtension);
		
		if (fileType == FILEEXT_UNKNOWN)
		{
			log_msg(ERROR, "Request cannot be fulfilled: unsupported file type.");
			
			char *errorResponse = craftStandardErrorResponse(HTTP_ERROR_501_NOT_IMPLEMENTED);
			
			if (!errorResponse)
			{
				log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
				
				close(tcp_client_socket);
				
				break;
			}
			
			sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
			
			free(tcp_client_message);
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		char *mimeType = getMIMEType(fileType);
		
		if (mimeType == NULL)
		{
			log_msg(ERROR, "Request cannot be fulfilled: file type has no associated MIME type!");
			
			char *errorResponse = craftStandardErrorResponse(HTTP_ERROR_500_INTERNAL_SERVER_ERROR);
			
			if (!errorResponse)
			{
				log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
				
				close(tcp_client_socket);
				
				break;
			}
			
			sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
			
			free(tcp_client_message);
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		char *ptrLine = strtok(tcp_client_message, "\r\n");
		
		if (ptrLine == NULL)
		{
			log_msg(WARN, "No header fields provided.");
		}
		
		int error = 0;
		
		while ((ptrLine = strtok(NULL, "\r\n")) != NULL)
		{
			char *lineCopy = (char *)calloc(strlen(ptrLine), sizeof(char));
			
			strncpy(lineCopy, ptrLine, strlen(ptrLine));
			
			char *ptrField = strtok(lineCopy, ": ");
			
			if (ptrField == NULL)
			{
				log_msg(ERROR, "Request is not well formed. Responding with 400 Bad Request.");
				
				char *errorResponse = craftStandardErrorResponse(HTTP_ERROR_400_BAD_REQUEST);
				
				if (!errorResponse)
				{
					log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
					
					close(tcp_client_socket);
					
					break;
				}
				
				sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
				
				free(lineCopy);
				
				free(fileData);
				
				free(tcp_client_message);
				tcp_client_message = (char *)malloc(sizeof(char) * 512);
				
				continue;
			}
			
			int httpFieldType = parseHTTPFieldName(ptrField);
			
			if (httpFieldType == UNKNOWN_FIELD)
			{
			}
			
			switch (httpFieldType)
			{
			case HTTP_FIELD_CONNECTION:
			{
				
			}
			
			case HTTP_FIELD_ACCEPT_LANGUAGE:
			{
				ptrField = strtok(NULL, "\r\n");
				
				char **tokens = NULL;
				double *weights = NULL;
				
				int languageValue = parseHTTPFieldMultipartWeightedValue(HTTP_FIELD_ACCEPT_LANGUAGE, ptrField, &tokens, &weights);
				
				// We don't need the tokens or weights, really.
				
				free(tokens);
				free(weights);
				
				if (!(languageValue == HTTP_CONTENT_ACCEPT_LANGUAGE_EN || languageValue == HTTP_CONTENT_ACCEPT_LANGUAGE_MULTIPLE))
				{
					if (languageValue == HTTP_CONTENT_ACCEPT_LANGUAGE_OTHER)
					{
						log_msg(WARN, "Client is not accepting English, though it is the only language served by this server. Ignored.");
						
						free(lineCopy);
						
						continue;
					}
					else
					{
						log_msg(ERROR, "Accept-Language values did not parse properly; assuming malformed. Returning 400 Bad Request.");
						
						free(lineCopy);
						
						error = HTTP_ERROR_400_BAD_REQUEST;
						
						break;
					}
				}
				
				continue;
			}
			
			case HTTP_FIELD_CONTENT_LANGUAGE:
			case HTTP_FIELD_CONTENT_LENGTH:
			case HTTP_FIELD_CONTENT_TYPE:
			{
				log_msg(WARN, "Content was sent but is not accepted by this server.");
				
				free(lineCopy);
				
				continue;
			}
			
			case HTTP_FIELD_USER_AGENT:
			case HTTP_FIELD_HOST:
			case UNKNOWN_FIELD:
			default:
			{
				log_msg(WARN, "Request field is unknown or ignored.");
				
				free(lineCopy);
				
				continue;
			}
			}
		}
		
		if (error != 0)
		{
			char *errorResponse = craftStandardErrorResponse(error);
			
			if (!errorResponse)
			{
				log_msg(ERROR, "Fatal error! Closing connection immediately and exiting.");
				
				close(tcp_client_socket);
				
				break;
			}
			
			sendAndConsume(tcp_client_socket, errorResponse, strlen(errorResponse));
			
			free(fileData);
			
			free(tcp_client_message);
			tcp_client_message = (char *)malloc(sizeof(char) * 512);
			
			continue;
		}
		
		char *headersBuffer = craftStandardResourceResponse(mimeType, readSize);
		
		int totalLen = strlen(headersBuffer) + readSize + 2;
		
		char *responseBuffer = (char *)calloc(totalLen + 1, sizeof(char));
		
		snprintf(responseBuffer, totalLen, "%s\r\n", headersBuffer);
		
		memcpy(responseBuffer + strlen(responseBuffer), fileData, readSize);
		
		free(fileData);
		
		responseBuffer[totalLen] = '\0';
		
		sendAndConsume(tcp_client_socket, responseBuffer, totalLen + 1);
		
		free(tcp_client_message);
		tcp_client_message = (char *)malloc(sizeof(char) * 512);
	}
	
	log_msg(INFO, "Closing client socket.");
	
	close(tcp_client_socket);
}

int main(int argc, char **argv)
{
	int httpPort = 60001;
	
	if (argc == 2)
	{
		if (!validateInteger(argv[1]))
		{
			log_msg(ERROR, "Could not start HTTP server: did not provide a valid integer port number. Please specify a port number between 60001 and 60099, inclusive.");
			
			return -1;
		}
		
		httpPort = strtol(argv[1], (char **)NULL, 10);
		
		if (httpPort < 60001 || httpPort > 60099)
		{
			log_msg(ERROR, "Could not start HTTP server: did not provide a valid integer port number. Please specify a port number between 60001 and 60099, inclusive.");
			
			return -1;
		}
	}
	
	// 1. Create the server socket.
	
	log_msg(INFO, "Creating server socket.");
	
	int tcp_server_socket;					// Socket descriptor
	tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0);	// Open TCP socket
	
	// 2-a. Define the server IP and port.
	
	log_msg(INFO, "Defining server IP and port.");
	
	struct sockaddr_in tcp_server_address;			// Address structure
	tcp_server_address.sin_family = AF_INET;		// Define address family
	tcp_server_address.sin_port = htons(httpPort);		// Define address port
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
	
	listen(tcp_server_socket, 1);
	
	// 4. Accept a new connection.
	
	int tcp_client_socket;
	tcp_client_socket = accept(tcp_server_socket, NULL, NULL);
	
	log_msg(INFO, "Found request: opening connection.");

	handleRequests(tcp_server_socket, tcp_client_socket);
	
	log_msg(INFO, "Done accepting connections! Exiting now.");
	
	return 0;
}
