#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

char	*get_next_line(int fd) 
{
	char *s;;
	char *c;

	if (BUFFER_SIZE < 1)
		return (NULL);
	// if (fd < 0)
	// 	return (NULL);
	s = (char *)malloc(10000);
	if (s == NULL)
		return (NULL);
	c = s;
	while (read(fd, c, 1) > 0 && *c++ != '\n');
	if (c <= s)
	{
		free(s);
		return (NULL);
	}
	*c = 0;
	return (s);
}

int main()
{
	int fd = open("text.txt", O_RDONLY);
	printf("main : [%s]\n", get_next_line(fd));
}