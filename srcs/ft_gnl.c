/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_gnl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:44:57 by adpachec          #+#    #+#             */
/*   Updated: 2025/03/11 13:30:55 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#define BUFFER_SIZE 1000

static int	ft_findnl(char *s)
{
	int	i;

	if (!s)
		return (-1);
	i = 0;
	while (s[i] != '\n' && s[i] != '\0')
		i++;
	if (s[i] != '\0')
		return (i);
	return (-1);
}

static char	*ft_adjust_buff(char *s, int pos_nl, int i)
{
	int		j;
	char	*aux;
	int		len_s;

	if (!s || (pos_nl == -1 && i == 0))
		return (NULL);
	len_s = ft_strlen(s);
	while (i < len_s)
		s[i++] = 0;
	if (pos_nl == -1)
		pos_nl = ft_strlen(s);
	else
		++pos_nl;
	aux = (char *)calloc(pos_nl + 1, sizeof(char));
	if (!aux)
		return (NULL);
	j = -1;
	while (++j < pos_nl)
		aux[j] = s[j];
	aux[j] = 0;
	return (aux);
}

static char	*ft_strjoin(char *s1, char *s2)
{
	char	*dest;
	int		len_s1;
	int		len_total;
	int		i;

	len_s1 = ft_strlen(s1);
	len_total = len_s1 + ft_strlen(s2);
	if (len_total == 0)
	{
		free(s1);
		return (NULL);
	}
	dest = (char *)calloc ((len_total + 1), sizeof(char));
	if (!dest)
		return (NULL);
	i = -1;
	while (++i < len_s1)
		dest[i] = s1[i];
	--i;
	while (++i < len_total)
		dest[i] = s2[i - len_s1];
	dest[i] = '\0';
	free(s1);
	return (dest);
}

static char	*ft_remain_buff(char *s, int pos_nl, int i)
{
	int		j;
	int		len;
	char	*aux;

	if (!s || pos_nl == -1 || i == -1)
		return (NULL);
	len = ft_strlen(s);
	aux = (char *)calloc((len - pos_nl), sizeof(char));
	if (!aux)
		return (NULL);
	j = 0;
	while (j < len - pos_nl - 1)
	{
		aux[j] = s[pos_nl + j + 1];
		j++;
	}
	aux[j] = 0;
	return (aux);
}

static char	*ft_read_line(char *buff, char *str, int pos_nl, int i)
{
	char	*line;
	char	*aux;

	if (i == -1)
		return (NULL);
	aux = ft_adjust_buff(str, pos_nl, i);
	line = ft_strjoin(buff, aux);
	free(aux);
	return (line);
}

static char	*ft_init_str(int fd, char *buff, int *pos_nl)
{
	char	*str;

	if (fd == -1 || BUFFER_SIZE < 1 || read(fd, 0, 0) < 0)
	{
		free(buff);
		return (NULL);
	}
	*pos_nl = ft_findnl(buff);
	if (*pos_nl == -1)
		str = (char *)calloc(BUFFER_SIZE + 1, sizeof(char));
	else
		str = ft_strjoin(buff, NULL);
	return (str);
}

static int	ft_init_i(int fd, char *str, int *pos_nl)
{
	int	i;

	if (*pos_nl == -1)
	{
		i = read(fd, str, BUFFER_SIZE);
		*pos_nl = ft_findnl(str);
	}
	else
		i = ft_strlen(str);
	return (i);
}

char	*get_next_line(int fd, char **buff)
{
	char		*str;
	char		*line;
	int			pos_nl;
	int			i;

	pos_nl = 0;
	str = ft_init_str(fd, *buff, &pos_nl);
	if (!str || pos_nl != -1)
		*buff = NULL;
	i = ft_init_i(fd, str, &pos_nl);
	while (i == BUFFER_SIZE && pos_nl == -1)
	{
		*buff = ft_strjoin(*buff, str);
		i = read(fd, str, BUFFER_SIZE);
		pos_nl = ft_findnl(str);
	}
	line = ft_read_line(*buff, str, pos_nl, i);
	*buff = ft_remain_buff(str, pos_nl, i);
	if (*buff == 0 || i == -1)
		free(*buff);
	free(str);
	return (line);
}
