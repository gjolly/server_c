#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <argp.h>

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 	8080

/* Used by main to communicate with parse_opt. */
struct arguments
{
	char* 	args[1];
	char* 	address;
	int 	port;
};

struct arguments arguments;

int parse_option(int argc, char **argv);

#endif
