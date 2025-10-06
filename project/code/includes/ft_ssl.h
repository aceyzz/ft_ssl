#ifndef FT_SSL_H
# define FT_SSL_H

/* std */
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdint.h>

/* custom */
# include "colors.h"
# include "libft.h"

/* algo */
# include "md5.h"
# include "sha256.h"

/* exit.c */
void	exit_msg(int fd, char *message, int exit_code);

#endif
