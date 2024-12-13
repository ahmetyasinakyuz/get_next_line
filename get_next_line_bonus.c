/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:39:34 by aakyuz            #+#    #+#             */
/*   Updated: 2024/10/30 14:24:48 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_strchr(char *s, int c)
{
	unsigned int	i;
	char			cc;

	cc = (char) c;
	i = 0;
	while (s[i])
	{
		if (s[i] == cc)
			return ((char *) &s[i]);
		i++;
	}
	if (s[i] == cc)
		return ((char *) &s[i]);
	return (NULL);
}

static char	*set_line(char *line)
{
	char	*left_c;
	size_t	i;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == 0 || line[1] == 0)
		return (NULL);
	left_c = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (*left_c == 0)
	{
		free(left_c);
		left_c = NULL;
	}
	line[i + 1] = 0;
	return (left_c);
}

static char	*fill_line_buffer(int fd, char *left_c, char *buffer)
{
	ssize_t	b_read;
	char	*tmp;

	b_read = 1;
	while (b_read > 0)
	{
		b_read = read(fd, buffer, BUFFER_SIZE);
		if (b_read == -1)
		{
			free(left_c);
			return (NULL);
		}
		else if (b_read == 0)
			break ;
		buffer[b_read] = 0;
		if (!left_c)
			left_c = ft_strdup("");
		tmp = left_c;
		left_c = ft_strjoin(tmp, buffer);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (left_c);
}

static char	*ft_free(char **left_c, int fd)
{
	free(left_c[fd]);
	left_c[fd] = NULL;
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*left_c[OPEN_MAX];
	char		*line;
	char		*buffer;

	if (fd < 0 || (unsigned long long)BUFFER_SIZE <= 0
		|| fd >= OPEN_MAX || read(fd, 0, 0) < 0)
	{
		if (fd >= 0 && fd < OPEN_MAX && left_c[fd])
		{
			free(left_c[fd]);
			left_c[fd] = NULL;
		}
		return (NULL);
	}
	buffer = (char *)malloc(((unsigned long long)BUFFER_SIZE + 1)
			* sizeof(char));
	if (!buffer)
		return (NULL);
	line = fill_line_buffer(fd, left_c[fd], buffer);
	free(buffer);
	if (!line)
		return (ft_free(left_c, fd));
	left_c[fd] = set_line(line);
	return (line);
}
