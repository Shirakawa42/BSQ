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

int		check_fill(char *map, int size, char e, char f, char fm, int nbline, int linesize, int b, int ten)
{
	int		i;
	int		x;
	int		y;
	int		error;
	int		faster;
	int		linesizewithn;
	int		nbchar;
	int		sizey;

	i = 0;
	error = 1;
	linesizewithn = linesize + 1;
	nbchar = linesizewithn * nbline;
	sizey = size * linesizewithn;
	while (i < nbchar && error == 1)
	{
		error = 0;
		x = 0;
		while (x < size)
		{
			y = 0;
			while (y < sizey)
			{
				if (i < nbchar && map[i + x + y] != e)
				{
					i += x;
					y = sizey;
					error = 1;
				}
				y += linesizewithn;
			}
			x++;
		}
		i++;
	}
	if (error == 1)
		return (-1);
	else
	{
		sizey = size * 2;
		if (b == 0)
			fill_and_print(map, &map[i - 1], size, fm, e, linesize);
		else
		{
			while (ten > 0 && check_fill(map, size + ten, e, f, fm, nbline, linesize, 0, 0) == -1)
				ten--;
			if (ten == 0)
				fill_and_print(map, &map[i - 1], size, fm, e, linesize);

		}
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
	int		ten;

	i = 0;
	while (map[i] != '\n')
		i++;
	size = opti_size(map, nbline, filled, i);
	if (nbline > i)
		ten = i / (i / 10);
	else
		ten = nbline / (nbline / 10);
	if (ten < 2)
		ten = 2;
	while (size > 0)
	{
		if ((result = check_fill(map, size, empty, filled, filledbyme, nbline, i, 1, ten)) == -1)
			size -= ten;
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
		map++;
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
