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

int	len_list(t_list **l)
{
	t_list	*cur;
	int		len = 0;

	cur = *l;
	while(cur != NULL)
	{
		len++;
		cur = cur->nxt;
	}
	return (len);
}

char *list_to_str_(t_list **l)
{
	t_list	*cur;
	char	*str;
	int		i;

	str = (char *)malloc(len_list(l) + 1);
	if (str == NULL)
		return (NULL);
	printf("malloc str     in list_to_str  %p\n", str);
	cur = *l;
	while(cur != NULL && cur->nxt != NULL)
		cur = cur->nxt;
	i = 0;
	while(cur != NULL)
	{
		str[i++] = cur->c;
		cur = cur->prv;
	}
	str[i] = '\0';
	return (str);
}

void print_list(t_list **l)
{
	t_list *cur;

	printf("list %p [%s] len = %d :\n", l, list_to_str_(l), len_list(l));
	cur = *l;
	while(cur != NULL && cur->nxt != NULL)
		cur = cur->nxt;
	while(cur != NULL)
	{
		printf(" %14p <- %p %c -> %p\n", cur->prv, cur, cur->c, cur->nxt);
		cur = cur->prv;
	}
}

void	*free_list(t_list ***l)
{
	t_list *cur;
	t_list *to_free;

	if (*l != NULL)
	{
		cur = **l;
		while(cur != NULL)
		{
			to_free = cur;
			cur = cur->nxt;
			printf("free   to_free in free_list    %p\n", to_free);
			free(to_free);
		}
		printf("free   l       in free_list    %p\n", *l);
		free(*l);
	}
}

void	*free_all_and_return_null(t_list ***l, char *buf)
{
	free_list(l);
	if (buf != NULL)
	{
		printf("free   buf     in free_all_and_return_null %p\n", buf);
		free (buf);
	}
	return (NULL);
};

char	*free_all_and_return_str(t_list ***l, char *buf)
{
	char	*list_to_str = NULL;

	if (*l != NULL)
	{
		list_to_str = list_to_str_(*l);
		if (list_to_str == NULL)
			return (NULL);
	}	
	free_list(l);
	if (buf != NULL)
	{
		printf("free   buf     in free_ret_str %p\n", buf);
		free (buf);
	}
	return (list_to_str);
}

int	add_to_list(t_list **l, char c)
{
	t_list *new = NULL;

	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (-1);
	printf("malloc new     in add_to_listr %p\n", new);
	new->c = c;
	new->prv = NULL;
	new->nxt = *l;
	if (*l != NULL)
		(*l)->prv = new;
	*l = new;
	return (0);
}

char	*get_next_line(int fd)
{
	t_list			**l = NULL;
	static char		*buf = NULL;
	static int		i = 0;
	int				nb_read;
	char			*list_to_str;

	if (BUFFER_SIZE < 1)
		return (free_all_and_return_null(&l, buf));
	l = (t_list **)malloc(sizeof(t_list *));
	if (l == NULL)
		return (free_all_and_return_null(&l, buf));
	printf("malloc l       in gnl          %p\n", l);
	*l = NULL;
	if (buf == NULL)
	{
		buf = (char *)malloc(BUFFER_SIZE);
		if (buf == NULL)
			return (free_all_and_return_null(&l, buf));
		printf("malloc buf     in gnl          %p\n", buf);
	}
	while (1)
	{
		nb_read = read(fd, buf, BUFFER_SIZE);
		if (nb_read < 0) //// сымитировать
			return (free_all_and_return_null(&l, buf));
		if (nb_read == 0) // EOF
			return (free_all_and_return_str(&l, buf));
		while (1)
		{
			if (add_to_list(l, buf[i]) == -1)
				return (free_all_and_return_null(&l, buf));
			//print_list(l);
			if (buf[i] == '\n' && i == nb_read - 1) // EOF
				return (free_all_and_return_str(&l, buf)); ////
			if (buf[i] == '\n')
				return (free_all_and_return_str(&l, NULL)); ////
			if (i == nb_read - 1) // EOF
				return (free_all_and_return_str(&l, buf));
			if (i == BUFFER_SIZE - 1)
			{
				i = 0;
				break ;
			}
			i++;
		}
	}
}

int main()
{
	char *str;
	int fd = open("text.txt", O_RDONLY);

	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	printf("free   str     in main         %p\n", str);
	free(str);
}