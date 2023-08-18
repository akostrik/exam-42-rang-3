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
	char			buf[BUFFER_SIZE];
	int				beg_line;
	int				end_line;
	struct s_list	*prv; 
	struct s_list	*nxt; 
} t_list;

int	len_line(t_list **l)
{
	t_list	*cur;
	int		len = 0;

	cur = *l;
	while(cur != NULL)
	{
		//printf("cur = %p\n", cur);
		len += cur->end_line - cur->beg_line + 1;
		cur = cur->nxt;
	}
	return (len);
}

char *list_to_str_(t_list **l)
{
	t_list	*cur;
	char	*str;
	int		i;
	int		j;

	str = (char *)malloc(len_line(l) + 1);
	if (str == NULL)
		return (NULL);
	//printf("malloc str     in list_to_str  %p\n", str);
	cur = *l;
	while(cur != NULL && cur->nxt != NULL)
		cur = cur->nxt;
	i = 0;
	while(cur != NULL)
	{
		j =	cur->beg_line;
		while (j <= cur->end_line)
		{
			str[i] = (cur->buf)[j];
			i++;
			j++;
		}
		cur = cur->prv;
	}
	str[i] = '\0';
	return (str);
}

void print_list(t_list **l)
{
	t_list *cur;
	char *list_to_str = NULL;

	list_to_str = list_to_str_(l);
	printf("LIST %p [%s] len = %d :\n", l, list_to_str, len_line(l));
	free(list_to_str);
	cur = *l;
	while(cur != NULL && cur->nxt != NULL)
		cur = cur->nxt;
	while(cur != NULL)
	{
		printf(" %14p <- %p [%s] [%d %d]-> %p\n", cur->prv, cur, cur->buf, cur->beg_line, cur->end_line, cur->nxt);
		cur = cur->prv;
	}
}

char	*free_list_delete_list_return_str(t_list ***l)
{
	t_list	*cur;
	t_list	*to_free;
	char	*list_to_str = NULL;

	if (*l != NULL)
	{
		list_to_str = list_to_str_(*l);
		cur = **l;
		while(cur != NULL)
		{
			to_free = cur;
			cur = cur->nxt;
			//printf("free   elt     in free_list    %p\n", to_free);
			free(to_free);
		}
		//printf("free   l       in free_list    %p\n", *l);
		free(*l);
		*l = NULL;
	}
	return (list_to_str);
}

char	*free_list_return_str(t_list ***l)
{
	t_list *cur;
	t_list *to_free;
	char	*list_to_str = NULL;

	if (*l != NULL)
	{
		list_to_str = list_to_str_(*l);
		cur = **l;
		while(cur != NULL)
		{
			to_free = cur;
			cur = cur->nxt;
			//printf("free   elt     in free_list    %p\n", to_free);
			free(to_free);
			to_free = NULL;
		}
		//printf("free   l       in free_list    %p\n", *l);
		**l = NULL;
	}
	return (list_to_str);
}

char	*free_list_except_last_buf_return_str(t_list ***l)
{
	t_list *cur;
	t_list *to_free;
	char	*list_to_str = NULL;

	if (*l != NULL && **l != NULL)
	{
		list_to_str = list_to_str_(*l);
		cur = (**l)->nxt;
		while(cur != NULL)
		{
			to_free = cur;
			cur = cur->nxt;
			//printf("free   elt     in free_list    %p\n", to_free);
			free(to_free);
		}
	}
	return (list_to_str);
}

int	read_buf_to_list(t_list **l, int fd)
{
	t_list *new = NULL;

	new = (t_list *)malloc(sizeof(t_list));
	if (new == NULL)
		return (-1);
	//printf("malloc elt     in add_to_listr %p\n", new);
	new->prv = NULL;
	new->nxt = *l;
	new->beg_line = 0;
	new->end_line = read(fd, new->buf, BUFFER_SIZE) - 1;
	if (*l != NULL)
		(*l)->prv = new;
	*l = new;
	if (new->end_line < 0) //// сымитировать
		return (-1);
	return (0);
}

char	*get_next_line(int fd)
{
	static t_list	**l = NULL;
	static int		i = 0;

	if (BUFFER_SIZE < 1)
		return (NULL);
	if (l == NULL)
	{
		l = (t_list **)malloc(sizeof(t_list *));
		if (l == NULL)
			return (free_list_delete_list_return_str(&l), NULL);
		//printf("malloc l       in gnl          %p\n", l);
		*l = NULL;
	}
	while (1)
	{
		printf("i = %d\n", i);
		if (i == 0)
		{
			if (read_buf_to_list(l, fd) == -1) // [ EOF ] or error
			{
				printf("0\n");
				i = 0;
				return (free_list_delete_list_return_str(&l));
			}
		}
		if ((*l)->buf[i] == '\n' && i == 0 && (*l)->end_line == 0) // [ \n ] [ \n EOF ]
		{
			printf("1\n");
			i = 0;
			return (free_list_return_str(&l));
		}
		else if ((*l)->buf[i] == '\n' && i != (*l)->end_line) // [ A \n B C ]
		{
			printf("2\n");
			(*l)->beg_line = i + 1;
			i = (i + 1) % BUFFER_SIZE ;
			return (free_list_except_last_buf_return_str(&l));
		}
		else if ((*l)->buf[i] == '\n' && i == (*l)->end_line) // [ A B C \n ] [ A \n EOF ]
		{
			printf("3\n");
			i = 0;
			return (free_list_return_str(&l));
		}
		else if (i == (*l)->end_line && i != BUFFER_SIZE - 1) // [ A EOF ]
		{
			printf("4\n");
			i = 0;
			return (free_list_delete_list_return_str(&l));
		}
		i = (i + 1) % BUFFER_SIZE ;
	}
}

int main()
{
	char *str;
	int fd = open("text.txt", O_RDONLY);

	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	//printf("free   str     in main         %p\n", str);
	free(str);

	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	//printf("free   str     in main         %p\n", str);
	free(str);

	str = get_next_line(fd);
	printf("main : [%s]\n", str);
	//printf("free   str     in main         %p\n", str);
	free(str);
}