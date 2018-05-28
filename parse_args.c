#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include "parse_args.h"

/* Program documentation. */
static char doc[] = "Server-C: a lightweigh server written in C";

/* The options we understand. */
static struct argp_option options[] = {
	{"ip-address", 	'a', "IP-address", 	0, "IP address to listen on" },
	{"port", 		'p', "TCP-port", 	0, "TCP port to listen on" },
	{ 0 }
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'a':
		arguments->address = arg;
      break;
    case 'p':
      arguments->port = atoi(arg);
      break;

	case ARGP_KEY_ARG:
		argp_usage (state);

      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, "", doc };

int
parse_option(int argc, char **argv)
{
  /* Default values. */
  arguments.address = DEFAULT_ADDRESS;
  arguments.port 	= DEFAULT_PORT;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  int err = argp_parse (&argp, argc, argv, 0, 0, &arguments);

  return err;
}
