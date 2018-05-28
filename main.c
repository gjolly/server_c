#include "server.h"
#include "parse_args.h"

int main(int argc, char** argv) {
	parse_option(argc, argv);

	return serve(arguments.address, arguments.port);
}
