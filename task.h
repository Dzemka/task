#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct s_country
{
	char	*str;
	int		start_x;
	int		start_y;
	int		end_x;
	int		end_y;
	int		count_city;
	int		count_day;
	int		full;
} t_country;

typedef struct s_tile
{
	int		country_index;
	size_t	*count_money;
} t_tile;

typedef struct s_task
{
	t_country	*country;
	t_tile		**map;
	int			width_map;
	int			height_map;
	int			country_count;
} t_task;
