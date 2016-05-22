/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nle-bret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/20 05:23:52 by nle-bret          #+#    #+#             */
/*   Updated: 2016/03/07 02:49:44 by mfroehly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		*ft_strcpy_limit(char *str, char n)
{
	int		i;
	char	*dst;

	i = 0;
	while (str[i] && str[i] != n)
		i++;
	dst = (char *)ft_memalloc(i + 1);
	i = 0;
	while (str[i] && str[i] != n)
	{
		dst[i] = str[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

static t_save	*ft_create_fd(int fd_pnum)
{
	t_save		*fd;

	fd = (t_save *)ft_memalloc(sizeof(t_save));
	fd->rest = ft_memalloc(1);
	fd->fd_num = fd_pnum;
	fd->next = NULL;
	return (fd);
}

static int		ft_save(t_save **s, char *buf, char **line)
{
	char	*eol;
	char	*tmp;

	if ((eol = ft_strchr(buf, '\n')) != NULL && eol++)
	{
		if ((*s)->rest && ft_strchr((*s)->rest, '\n') == NULL)
			*line = ft_strjoin_free((*s)->rest, ft_strcpy_limit(buf, '\n'));
		else
			*line = ft_strcpy_limit(buf, '\n');
		tmp = (*s)->rest;
		(*s)->rest = ft_strdup(eol);
		ft_strdel(&tmp);
		return (1);
	}
	else
	{
		tmp = (*s)->rest;
		if ((*s)->rest)
			(*s)->rest = ft_strjoin((*s)->rest, buf);
		else
			(*s)->rest = ft_strdup(buf);
		ft_strdel(&tmp);
	}
	return (0);
}

static t_save	*ft_get_list(t_save **s, int fd)
{
	t_save			*lst;

	if (!*s)
		*s = ft_create_fd(fd);
	lst = *s;
	while (lst->next && lst->fd_num != fd)
		lst = lst->next;
	if (lst->next == NULL && lst->fd_num != fd)
		lst->next = ft_create_fd(fd);
	lst = *s;
	while (lst && lst->fd_num != fd)
		lst = lst->next;
	return (lst);
}

int				get_next_line(int const fd, char **line)
{
	char			buf[BUFF_SIZE + 1];
	int				ret;
	static t_save	*s;
	t_save			*lst;

	if (!line)
		return (-1);
	lst = ft_get_list(&s, fd);
	if (lst && lst->rest && ft_strchr(lst->rest, '\n') &&
			ft_save(&lst, lst->rest, line))
		return (1);
	while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if (ft_save(&lst, buf, line))
			return (1);
	}
	if (ret == 0 && lst->rest && (*line = ft_strdup(lst->rest)) != NULL)
	{
		ft_strdel(&lst->rest);
		return (1);
	}
	if (ret == -1)
		return (-1);
	return (0);
}
