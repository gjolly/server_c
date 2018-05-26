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

#define BACKLOG 	100
#define RESPFILE 	"response.http"
#define SIZEMAXRESP 200

int serve(const char* ip_addr, int port) {
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
	char buf[SIZEMAXRESP];
	int respfile, connSock, nb_read;
	socklen_t addrlen = (socklen_t)sizeof(addr); 
	while (1) {
		// read resp file
		respfile = open(RESPFILE, O_RDONLY);
		nb_read = read(respfile, buf, sizeof(buf));
		close(respfile);

		connSock = accept(sock, (struct sockaddr*)&addr, &addrlen);
		write(connSock, buf, nb_read);
		close(connSock);
	}
	
	return 0;
}
