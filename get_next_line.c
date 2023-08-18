#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct	s_list
{
	char			c;
	struct s_list	*prv; 
	struct s_list	*nxt; 
} t_list;

void print_list(t_list **l)
{
	t_list *cur;

	printf("LIST %p %p:\n", l, *l);
	cur = *l;
	while(cur != NULL)
	{
		printf(" %14p <- %p [%c] -> %p\n", cur->prv, cur, cur->c, cur->nxt);
		cur = cur->nxt;
	}
}

int	len_line(t_list **l)
{
	t_list	*cur;
	int		len = 0;

	cur = *l;
	while(cur != NULL)
	{
		len++;
		if (cur->c == '\n')	
			break;
		cur = cur->nxt;
	}
	return (len);
}

char	*take_a_line_of_the_list(t_list ***l)
{
	t_list	*cur;
	t_list	*to_free;
	char	*line = NULL;
	int		i;
	int		to_break = 0;

	if (*l != NULL)
	{
		line = (char *)malloc(len_line(*l) + 1);
		if (line == NULL)
			return (NULL);
		printf("malloc str     in list_to_str  %p\n", line);
		cur = **l;
		i = 0;
		while(cur != NULL && to_break == 0)
		{
			line[i] = cur->c;
			if (cur->c == '\n')	
				to_break = 1;
			i++;
			to_free = cur;
			cur = cur->nxt;
			**l = cur;
			printf("free   elt     in free_list    %p %c\n", to_free, to_free->c);
			free(to_free);
		}
		line[i] = '\0';
	}
	return (line);
}

void	free_list(t_list ***l)
{
	t_list	*cur;
	t_list	*to_free;

	if (*l != NULL)
	{
		cur = **l;
		while(cur != NULL)
		{
			to_free = cur;
			cur = cur->nxt;
			printf("free   elt     in free_list    %p %c\n", to_free, to_free->c);
			free(to_free);
		}
	}
	printf("free   l       in free_list    %pc\n", l);
	free(*l);
}

int	put_char_to_list(t_list ***l, char c)
{
	t_list	*new = NULL;
	int		nb_read;
	t_list	*cur;

	cur = **l;
	while(cur != NULL && cur->nxt != NULL)
		cur = cur->nxt;
	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (-1);
	printf("malloc elt     in add_to_listr %p\n", new);
	new->c = c;
	new->nxt = NULL;
	new->prv = cur;
	if (**l == NULL)
		**l = new;
	else
		cur->nxt = new;
	return (0);
}

int	put_buf_to_list(t_list ***l, int fd)
{
	t_list	*new = NULL;
	char	buf[BUFFER_SIZE];
	int		nb_read;
	int		i;

	nb_read = read(fd, buf, BUFFER_SIZE);
	if (nb_read < 0) //// сымитировать
		return (-1);
	i = 0;
	while (i < nb_read)
		put_char_to_list(l, buf[i++]);
	return (nb_read);
}

char	*get_next_line(int fd)
{
	static t_list	**l = NULL;
	static t_list	*cur = NULL;
	char			*line;

	if (BUFFER_SIZE < 1)
		return (NULL);
	if (l == NULL)
	{
		l = (t_list **)malloc(sizeof(t_list *));
		if (l == NULL)
			return (NULL);
		*l = NULL;
		printf("malloc l       in gnl          %p\n", l);
		if (put_buf_to_list(&l, fd) <= 0) // [ EOF ] or error
			return (free_list(&l), l = NULL, NULL);
		cur = *l;
	}
	while (1)
	{
		if (cur->nxt == NULL && cur->c != '\n')
		{
			if (put_buf_to_list(&l, fd) <= 0) // [ EOF ] or error
			{
				line = take_a_line_of_the_list(&l); // free list
				return (free_list(&l), l = NULL, line);
			}
			cur = *l;
		}
		if (cur->c == '\n')
			return (take_a_line_of_the_list(&l));
		cur = cur->nxt;
	}
}

int main()
{
	char *str = NULL;
	int fd = open("text.txt", O_RDONLY);

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		printf("free   str     in main         %p\n\n", str);
		free(str);
	}

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		printf("free   str     in main         %p\n\n", str);
		free(str);
	}

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		printf("free   str     in main         %p\n\n", str);
		free(str);
	}

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		printf("free   str     in main         %p\n\n", str);
		free(str);
	}

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		printf("free   str     in main         %p\n\n", str);
		free(str);
	}

}
