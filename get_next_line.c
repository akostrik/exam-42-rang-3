// #include <unistd.h>
// #include <stdlib.h>
// char *gnl(int fd) {
//     char *s = mallo c(10000), *c = s;
//     while (read(fd, c, 1) > 0 && *c++ != '\n');
//     return c > s ? (*c = 0, s) : (fre e(s), NULL);
// }

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "get_next_line.h"

typedef struct	s_list
{
	char			c;
	struct s_list	*nxt; 
} t_list;

void print_list(t_list **l, t_list *cur_g, char *msg)
{
	t_list *cur;

	printf("LIST %14s : %14p : %p : ", msg, l, l == NULL ? 0 : *l);
	printf("cur_g=%p\n", cur_g);
	if (l == NULL)
		return ;
	cur = *l;
	while(cur != NULL)
	{
		printf("  %p [%c] -> %p\n", cur, cur->c == '\n' ? 'n' : cur->c, cur->nxt);
		cur = cur->nxt;
	}
	printf("\n");
}

int	put_char_to_list(t_list **l, char c)
{
	t_list	*new = NULL;
	int		nb_read;
	t_list	*cur;

	cur = *l;
	while(cur != NULL && cur->nxt != NULL)
		cur = cur->nxt;
	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (-1);
	new->c = c;
	// printf("mal elt  %p %c\n", new, new->c);
	new->nxt = NULL;
	if (*l == NULL)                            ////
		*l = new;
	else
		cur->nxt = new;
	return (0);
}

int	put_buf_to_list(t_list **l, t_list **cur_g, int fd)
{
	char	buf[BUFFER_SIZE];
	int		nb_read;
	int		i = 0;

	nb_read = read(fd, buf, BUFFER_SIZE);
	while (nb_read > 0 && i < nb_read)           ////
		if (put_char_to_list(l, buf[i++]) == -1)
			return (-1);
	*cur_g = *l;                                ////
	return (nb_read);
}

int	there_is_a_line_with_n_in_the_list(t_list **l)
{
	t_list	*cur;

	cur = *l;
	while(cur != NULL)
	{
		if (cur->c == '\n')	
			return (1);
		cur = cur->nxt;
	}
	return (0);                                  ////
}

int	len_line(t_list **l)
{
	t_list	*cur;
	int		len = 0;

	cur = *l;
	while(cur != NULL)
	{
		if (cur->c == '\n')
			return (len + 1);                      ////
		cur = cur->nxt;
		len++;
	}
	return (len);                                  ////
}

char	*get_a_line_from_the_list(t_list **l, t_list **cur_g)
{
	t_list	*cur_l;
	t_list	*to_free;
	char	*line = NULL;
	int		i;
	int		to_break = 0;

	if (l == NULL || len_line(l) == 0)             ////
		return (NULL);
	line = (char *)malloc(len_line(l) + 1);
	if (line == NULL)
		return (NULL);
	cur_l = *l;
	i = 0;
	while(cur_l != NULL && to_break == 0)
	{
		line[i] = cur_l->c;
		if (cur_l->c == '\n')	
			to_break = 1;                           ////
		i++;
		to_free = cur_l;
		cur_l = cur_l->nxt;
		// printf("fre elt  %p %c\n", to_free, to_free->c);
		free(to_free);
		to_free = NULL;
	}
	line[i] = '\0';
	*l = cur_l;                                     ////
	*cur_g = *l;                                    ////
	return (line);
}

t_list **initialize_list(t_list ***l, t_list **cur_g)
{
	*l = (t_list **)malloc(sizeof(t_list *));
	// printf("mal list  %p\n", l);
	if (*l == NULL)
		return (NULL);
	**l = NULL;
	*cur_g = **l;
	return(*l);
}

int	end_of_list_reached(t_list **l, t_list **cur_g)
{
	return (*l == NULL || *cur_g == NULL);
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
			// printf("fre elt  %p\n", to_free);
			free(to_free);
			to_free = NULL;
		}
	}
	free(*l);
	// printf("fre list %p\n", *l);
	*l = NULL;                         ////
}

char	*get_next_line(int fd)
{
	static t_list	**l = NULL;
	static t_list	*cur_g = NULL;                    ////
	static int		E0F_reached = 0;
	char			*line;
	int				nb_read;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (l == NULL)
		l = initialize_list(&l, &cur_g);
	while (1)
	{
		if (there_is_a_line_with_n_in_the_list(l))
			return (get_a_line_from_the_list(l, &cur_g));
		if (end_of_list_reached(l, &cur_g))
			E0F_reached = (put_buf_to_list(l, &cur_g, fd) < 1); // [ EOF ] error
		if (!end_of_list_reached(l, &cur_g) && E0F_reached)
			return (get_a_line_from_the_list(l, &cur_g));
		if (end_of_list_reached(l, &cur_g) && E0F_reached)
			return (free_list(&l), NULL);
		cur_g = cur_g->nxt;
	}
}

int main()
{
	char *str = NULL;
	int fd = open("text.txt", O_RDONLY);
	int i;

	printf("\n");
	i = 0;
	while(i++ < 15)
	{
		str = NULL;
		str = get_next_line(fd);
		printf("%s", str);
		if (str != NULL)
			free(str);
	}
	printf("\n");
}
