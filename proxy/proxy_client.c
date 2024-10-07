#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define NON_EXISTENT_IP "192.168.3.3" // Example non-existent IP address
#define SERVER_IP "192.168.3.4" // Example server IP address

int main() {
	int sockfd;
	struct sockaddr_in server_addr, connect_addr;

	// Create socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Set IP_TRANSPARENT socket option
	int optval = 1;
	if (setsockopt(sockfd, SOL_IP, IP_TRANSPARENT, &optval, sizeof(optval)) < 0) {
		perror("Setting IP_TRANSPARENT failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// Set up server address structure for binding
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	// Convert IP address from text to binary form
	if (inet_pton(AF_INET, NON_EXISTENT_IP, &server_addr.sin_addr) <= 0) {
		perror("Invalid address/ Address not supported");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// Bind the socket to the non-existent IP address
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Socket successfully bound to non-existent IP address\n");

	// Set up server address structure for connecting
	memset(&connect_addr, 0, sizeof(connect_addr));
	connect_addr.sin_family = AF_INET;
	connect_addr.sin_port = htons(PORT);

	// Convert server IP address from text to binary form
	if (inet_pton(AF_INET, SERVER_IP, &connect_addr.sin_addr) <= 0) {
		perror("Invalid server address/ Address not supported");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// Connect to the server
	if (connect(sockfd, (struct sockaddr *)&connect_addr, sizeof(connect_addr)) < 0) {
		perror("Connection to the server failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Connected to the server\n");

	// Send a message to the server
	const char *message = "Hello, Server!";
	if (send(sockfd, message, strlen(message), 0) < 0) {
		perror("Send failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Message sent to the server\n");

	// Receive a message from the server
	char buffer[1024] = {0};
	int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received < 0) {
		perror("Receive failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Message received from the server: %s\n", buffer);
	// Close the socket
	close(sockfd);
	return 0;
}