#include "server.h"

#define ADDR "127.0.0.1"
#define PORT 8080

int main() {
	return serve(ADDR, PORT);
}
