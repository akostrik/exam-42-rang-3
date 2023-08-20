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

/*void print_list(t_list **l)
{
	t_list *cur;

	printf("LIST %p %p:\n", l, *l);
	if (*l == NULL)
		printf("vide\n");
	cur = *l;
	while(cur != NULL)
	{
		printf(" %14p <- %p [%c] -> %p\n", cur->prv, cur, cur->c, cur->nxt);
		cur = cur->nxt;
	}
}*/

int	len_line(t_list **l)
{
	t_list	*cur;
	int		len = 0;

	cur = *l;
	while(cur != NULL)
	{
		len++;
		if (cur->c == '\n')	
			return (len);
		cur = cur->nxt;
	}
	return (len);
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
	new->c = c;
	new->nxt = NULL;
	new->prv = cur;
	if (**l == NULL)
		**l = new;
	else
		cur->nxt = new;
	return (0);
}

int	put_buf_to_list(t_list ***l, t_list **cur_g, int fd)
{
	t_list	*new = NULL;
	char	buf[BUFFER_SIZE];
	int		nb_read = -1;
	int		i = 0;

	nb_read = read(fd, buf, BUFFER_SIZE);
	while (nb_read > 0 && i < nb_read)
		if (put_char_to_list(l, buf[i++]) == -1)
			return (-1);
	*cur_g = **l;
	return (nb_read);
}

int	there_is_a_complet_line_in_the_list(t_list **l)
{
	t_list	*cur;

	cur = *l;
	while(cur != NULL)
	{
		if (cur->c == '\n')	
			return (1);
		cur = cur->nxt;
	}
	return (0);
}

char	*take_a_line_of_the_list(t_list ***l, t_list **cur_g)
{
	t_list	*cur_l;
	t_list	*to_free;
	char	*line = NULL;
	int		i;
	int		to_break = 0;

	if (*l != NULL && len_line(*l) > 0)
	{
		line = (char *)malloc(len_line(*l) + 1);
		if (line == NULL)
			return (NULL);
		cur_l = **l;
		i = 0;
		while(cur_l != NULL && to_break == 0)
		{
			line[i] = cur_l->c;
			if (cur_l->c == '\n')	
				to_break = 1;
			i++;
			to_free = cur_l;
			cur_l = cur_l->nxt;
			**l = cur_l;
			free(to_free);
			to_free = NULL;
		}
		line[i] = '\0';
	}
	*cur_g = **l;
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
			free(to_free);
		}
	}
	free(*l);
}

char	*get_next_line(int fd)
{
	static t_list	**l = NULL;
	static t_list	*cur_g = NULL;
	char			*line;
	int				nb_read;

	if (fd < 0 || BUFFER_SIZE < 1) ////
		return (NULL);
	if (l == NULL)
	{
		l = (t_list **)malloc(sizeof(t_list *));
		if (l == NULL)
			return (NULL);
		*l = NULL;
	}
	while (1)
	{
		if (there_is_a_complet_line_in_the_list(l))
			return (take_a_line_of_the_list(&l, &cur_g));
		if (*l == NULL || cur_g == NULL || (cur_g->c != '\n' && cur_g->nxt == NULL))
		{
			nb_read = put_buf_to_list(&l, &cur_g, fd);
			if (nb_read < BUFFER_SIZE) // [ A EOF ]
			{
				line = take_a_line_of_the_list(&l, &cur_g);
				if (nb_read <= 0) // [ EOF ] or error
					free_list(&l);
				return (line);
			}
		}
		// if (cur_g->c == '\n')
		// 	return (take_a_line_of_the_list(&l, &cur_g));
		// cur_g = cur_g->nxt;
	}
}

int main()
{
	char *str = NULL;
	int fd = open("text.txt", O_RDONLY);

	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		//printf("free   str     in main         %p\n\n", str);
		free(str);
		str = NULL;
	}

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		//printf("free   str     in main         %p\n\n", str);
		free(str);
		str = NULL;
	}

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		//printf("free   str     in main         %p\n\n", str);
		free(str);
		str = NULL;
	}

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		//printf("free   str     in main         %p\n\n", str);
		free(str);
		str = NULL;
	}

	str = NULL;
	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	if (str != NULL)
	{
		//printf("free   str     in main         %p\n\n", str);
		free(str);
		str = NULL;
	}
}
