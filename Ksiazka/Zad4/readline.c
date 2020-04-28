#include "lib.h"

#define MAXLINE 1024

static pthread_key_t	rl_key;
static pthread_once_t	rl_once = PTHREAD_ONCE_INIT;

static void readline_destructor(void *ptr)
{
	free(ptr);
}

static void readline_once(void)
{
	int		n;

	if((n = pthread_key_create(&rl_key, readline_destructor)) != 0)
	{
		errno = n;
		perror(errno);
		exit(EXIT_FAILURE);
		
	}
}

typedef struct {
	int	rl_cnt;
	char	*rl_bufptr;
	char	rl_buf[MAXLINE];
} Rline;

static ssize_t my_read(Rline *tsd, int fd, char *ptr)
{
	if(tsd->rl_cnt <= 0)
	{
again:
		if((tsd->rl_cnt = read(fd, tsd->rl_bud, MAXLINE)) < 0)
		{
			if(errno == EINTR)
				goto again;
			return(-1);
		} else if(tsd->rl_cnt == 0)
			return(0);

		tsd->rl_bufptr = tsd->rl_buf;
	}

	tsd->rl_cnt--;
	*ptr = *tsd->rl_bufptr++;
	return(1);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	int		n, rc, m;
	char		c, *ptr;
	Rline		*tsd;

	if((m = pthread_once(&rl_once, readline_once)) != 0)
	{
		errno = m;
		perror(errno);
		exit(EXIT_FAILURE);
	}

	if((tsd = pthread_getspecific(rl_key)) == NULL)
}


