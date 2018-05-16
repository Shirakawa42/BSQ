#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

void	fill_and_print(char *map, char *pos, int size, char fm, char e, int linesize)
{
	int		x;
	int		y;

	x = 0;
	while (x < size)
	{
		y = 0;
		while (y < size)
		{
			pos[x + (y * (linesize + 1))] = fm;
			y++;
		}
		x++;
	}
	ft_putstr(map);
}

int		check_fill(char *map, int size, char e, char f, char fm, int nbline, int linesize)
{
	int		i;
	char	*pos;
	int		x;
	int		y;
	int		error;
	int		faster;

	i = 0;
	error = 1;
	while (i < (linesize + 1) * nbline && error == 1)
	{
		pos = &map[i];
		error = 0;
		x = 0;
		if ((faster = linesize - (i % (linesize + 1))) >= size)
		{
			while (x < size)
			{
				y = 0;
				while (y < size)
				{
					if (i < (linesize + 1) * nbline && map[i + x + (y * (linesize + 1))] != e)
					{
						i += x;
						y = size;
						error = 1;
					}
					y++;
				}
				x++;
			}
		}
		else
		{
			error = 1;
			i += faster;
		}
		i++;
	}
	if (error == 1)
		return (-1);
	else
	{
		fill_and_print(map, pos, size, fm, e, linesize);
		return (i);
	}
}

int		opti_size(char *map, int nbline, char filled, int linesize)
{
	int		tmp;
	int		place;
	int		size;
	int		tmpsize;

	size = nbline;
	tmp = (linesize + 1) * (nbline / 2) + 1;
	place = 1;
	while (map[tmp] && map[tmp] != '\n')
	{
		if (map[tmp] == filled)
		{
			tmpsize = (place > linesize / 2) ? place + 1 : linesize - place + 1;
			size = (tmpsize < size) ? tmpsize : size;
		}
		tmp++;
		place++;
	}
	return (size);
}

void	solve_bsq(char *map, int nbline, char empty, char filled, char filledbyme)
{
	int		size;
	char	*opti;
	int		i;
	int		result;

	i = 0;
	while (map[i] != '\n')
		i++;
	size = opti_size(map, nbline, filled, i);
	while (size > 0)
	{
		if ((result = check_fill(map, size, empty, filled, filledbyme, nbline, i)) == -1)
			size--;
		else
			break ;
	}
}

void	read_then_solve(char *file)
{
	char	*map;
	char	buf[2048 * 2048];
	int		fd;
	int		i;
	int		nbline;
	char	*tmp;

	fd = open(file, O_RDONLY);
	map = (char*)malloc(sizeof(char) * 1);
	map[0] = '\0';
	while ((i = read(fd, buf, 2048 * 2048)))
	{
		buf[i] = '\0';
		map = (char*)realloc(map, sizeof(char) * (ft_strlen(map) + i + 1));
		map = ft_strcat(map, buf);
	}
	close(fd);
	nbline = ft_atoi(map);
	tmp = map;
	while (*map >= '0' && *map <= '9')
		*map++;
	solve_bsq(map + 4, nbline, map[0], map[1], map[2]);
	free(tmp);
}

int		main(int ac, char **av)
{
	int		i;

	if (ac < 2)
		return (0);
	i = 1;
	while (i < ac)
	{
		read_then_solve(av[i]);
		if (ac > 2)
			write(1, "\n", 1);
		i++;
	}
	return (0);
}
