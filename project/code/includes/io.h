#ifndef IO_H
# define IO_H

# include <stdint.h>
# include <stdlib.h>

int	io_read_fd(int fd, uint8_t **out, size_t *len);
int	io_read_file(const char *path, uint8_t **out, size_t *len);
int	io_read_stdin(uint8_t **out, size_t *len);

#endif
