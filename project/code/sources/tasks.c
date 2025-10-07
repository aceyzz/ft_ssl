#include "ft_ssl.h"

void	task_push_back(t_task **head, t_ikind kind, const char *label, const uint8_t *data, size_t len)
{
	t_task	*n;
	t_task	*it;

	n = (t_task *)malloc(sizeof(t_task));
	if (!n)
		exit(EXIT_FAILURE);
	n->kind = kind;
	n->label = label ? ft_strdup(label) : NULL;
	if (data && len > 0)
	{
		n->data = (uint8_t *)malloc(len);
		if (!n->data)
		{
			if (n->label) free(n->label);
			free(n);
			exit(EXIT_FAILURE);
		}
		ft_memcpy(n->data, data, len);
		n->len = len;
	}
	else
	{
		n->data = NULL;
		n->len = 0;
	}
	n->next = NULL;
	if (!*head)
	{
		*head = n;
		return ;
	}
	it = *head;
	while (it->next)
		it = it->next;
	it->next = n;
}

void	task_clear(t_task **head)
{
	t_task	*it;
	t_task	*nx;

	it = *head;
	while (it)
	{
		nx = it->next;
		if (it->label) free(it->label);
		if (it->data) free(it->data);
		free(it);
		it = nx;
	}
	*head = NULL;
}
