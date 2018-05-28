#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>

#define BACKLOG 		100
#define RESPFILE 		"response.test"
#define SIZEMAXRESP		 200
#define SIZE_MAX_HEADER 200

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

int
extract_host(char** header, char** host) {
	char* host_line = strstr(*header, "Host");
	if (!host_line)
		return -1;

	host_line = host_line + 6; // we just want the host name (Host: www.example.com)
	 													 //                             123456

	int i_char = 0;
	while (host_line[i_char] != '\0' && host_line[i_char] != '\n' && host_line[i_char] != '\r')
		i_char++;

	host_line[i_char] = '\0';

	*host = (char*)malloc(strlen(host_line) * sizeof(char));
	strcpy(*host, host_line);

	*header = *header + i_char + 1;

	return 0;
}

// TODO: add more contrain to check if the path is valid
int
extract_path(char** header, char** path) {
	// http request: GET /pub/WWW/TheProject.html HTTP/1.1

	char* start = *header + 4;
	int i_char = 0;

	while (start[i_char] != ' ' && start[i_char] != '\0')
		i_char++;

	start[i_char] = '\0';

	*path = (char*)malloc(strlen(start) * sizeof(char));
	strcpy(*path, start);

	*header = start + i_char + 1;

	return 0;
}

int
get_route_info(int sock, char** host, char** path) {
	char header[SIZE_MAX_HEADER];

	if (!read(sock, header, SIZE_MAX_HEADER)) {
		// if sizeof(header) is too small
		printf("SIZE_MAX_HEADER to small");
		return -1;
	}

	char* hd = (char*)header;

	// NOTE: the order of the two next call is very important
	// because header is modified
	if (extract_path(&hd, path)) {
		printf("[get_route_info] No path found\n");
		return -1;
	}

	if (extract_host(&hd, host)) {
		printf("[get_route_info] No host found\n");
		return -2;
	}

	return 0;
}

void*
conn_handler(void* sock_arg) {
	int conn = *(int*)sock_arg;

	char* host = NULL;
	char* path = NULL;

	if (get_route_info(conn, &host, &path))
		printf("[conn_handler] No host or path\n");
	else
		printf("%s%s\n", host, path);

	char buf[SIZEMAXRESP];
	int respfile, nb_read;

	// read resp file
	respfile = open(RESPFILE, O_RDONLY);
	nb_read = read(respfile, buf, sizeof(buf));
	close(respfile);

	write(conn, buf, nb_read);
	close(conn);
	free(host);
	free(path);

	return NULL;
}

int
serve(const char* ip_addr, int port) {
	printf("Starting server on %s:%i...\n", ip_addr, port);

	// create socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	// bind
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr);
	addr.sin_port = htons(port);
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr))) {
		perror(NULL);
	}

	// listen
	if (listen(sock, BACKLOG)) {
		perror(NULL);
	}

	// accept
	int connSock;
	socklen_t addrlen = (socklen_t)sizeof(addr);
	while (1) {
		connSock = accept(sock, (struct sockaddr*)&addr, &addrlen);

		pthread_t connection;
		pthread_create(&connection, NULL, conn_handler, &connSock);
	}

	return 0;
}
