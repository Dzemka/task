#include "task.h"

void fill_country(t_country *country, char **argv, t_task *task)
{
	int i;
	int j;
	char *s;

	j = -1;
	i = 1;
	while (++j < task->country_count)
	{
		country[j].str = strdup(argv[i]);
		country[j].start_x = atoi(argv[i + 1]);
		country[j].start_y = atoi(argv[i + 2]);
		country[j].end_x = atoi(argv[i + 3]);
		country[j].end_y = atoi(argv[i + 4]);
		country[j].count_city = (country[j].end_x - country[j].start_x + 1) * (country[j].end_y - country[j].start_y + 1);
		country[j].count_day = 0;
		country[j].full = 0;
		country[j].count_money = malloc(sizeof(int) * (task->country_count));
		i += 5;
	}
}

void fill_map(t_task *task)
{
	int x;
	int y;
	int country_index;
	int i;

	y = -1;
	while (++y < task->height_map)
	{
		x = -1;
		while (++x < task->width_map)
		{
			country_index = -1;
			task->map[y][x].country_index = -1;
			task->map[y][x].count_money = malloc(sizeof(size_t) * (task->country_count + 1));
			while (++country_index < task->country_count)
			{
				task->map[y][x].count_money[country_index] = 0;
				if (x >= task->country[country_index].start_x && x <= task->country[country_index].end_x && y >= task->country[country_index].start_y && y <= task->country[country_index].end_y)
				{
					task->map[y][x].count_money[country_index] = 1000000;
					task->map[y][x].country_index = country_index;
				}
			}
		}
	}
}

void init_map(t_task *task)
{
	int i;

	task->width_map = 0;
	task->height_map = 0;
	i = -1;
	while (++i < task->country_count)
	{
		if (task->country[i].end_x > task->width_map)
			task->width_map = task->country[i].end_x + 1;
		if (task->country[i].end_y > task->height_map)
			task->height_map = task->country[i].end_y + 1;
	}
	task->map = malloc(sizeof(t_tile *) * (task->height_map + 1));
	i = -1;
	while (++i < task->height_map)
		task->map[i] = malloc(sizeof(t_tile) * (task->width_map + 1));
	fill_map(task);
}

void transfer_money(t_tile *src, t_tile *dst, t_task *task)
{
	int	i;
	int	transfer_coins;

	i = -1;
	if (dst->country_index == -1 || src->country_index == -1)
		return ;
	while (++i < task->country_count)
	{
		printf("%ld\n", src->count_money[i]);
		if (src->count_money[i] > 1000)
		{
			transfer_coins = src->count_money[i] / 1000;
			dst->count_money[i] += transfer_coins;
			src->count_money[i] -= transfer_coins;
		}
	}
}

void pass_coin(int x, int y, t_task *task)
{
	t_tile *src;

	src = &task->map[y][x];
	if (y - 1 > 0)
		transfer_money(src, &task->map[y - 1][x], task);
	if (y + 1 < task->height_map)
		transfer_money(src, &task->map[y + 1][x], task);
	if (x - 1 > 0)
		transfer_money(src, &task->map[y][x - 1], task);
	if (x + 1 < task->width_map)
		transfer_money(src, &task->map[y][x + 1], task);
}

int is_full(t_task *task)
{
	int x;
	int y;
	int i;
	int j;
	int count_motif[task->country_count][task->country_count];
	int count_fill_country;
	int count_motif_on_country;

	j = -1;
	while (++j < task->country_count)
	{
		i = -1;
		while (++i < task->country_count)
			count_motif[j][i] = 0;
	}
	y = -1;
	while (++y < task->height_map)
	{
		x = -1;
		while (++x < task->width_map)
		{
			if (task->map[y][x].country_index != -1)
			{
				i = -1;
				while (++i < task->country_count)
				{
					if (task->map[y][x].count_money[i] > 0)
						count_motif[task->map[y][x].country_index][i]++;
				}
			}
		}
	}
	i = -1;
	count_fill_country = 0;
	while (++i < task->country_count)
	{
		count_motif_on_country = 0;
		j = -1;
		while (++j < task->country_count)
		{
			// printf("%d ",  task->country[i].count_money[j]);
			if (count_motif[i][j] == task->country[i].count_city)
				count_motif_on_country++;
		}
		// printf("\n");
		if (count_motif_on_country == task->country_count)
		{
			task->country[i].full = 1;
			count_fill_country++;
		}
	}
	if (count_fill_country == task->country_count)
		return (1);
	return (0);
}

void get_day_count(t_task *task)
{
	int i;
	int count_city;
	int x;
	int y;
	int index;
	int end;

	i = -1;
	end = 0;
	while (!end)
	{
		y = -1;
		while (++y < task->height_map)
		{
			x = -1;
			while (++x < task->width_map)
			{
				pass_coin(x, y, task);
				end = is_full(task);
				if (end)
					return;
			}
		}
		i = -1;
		while (++i < task->country_count)
		{
			if (task->country[i].full == 0)
				task->country[i].count_day++;
		}
	}
}

int main(int argc, char **argv)
{
	t_task *task;

	task = malloc(sizeof(t_task));
	if (argc < 6)
	{
		printf("error\n");
		return (1);
	}
	if ((argc - 1) % 5 != 0)
	{
		printf("error\n");
		return (1);
	}
	task->country_count = (argc - 1) / 5;
	task->country = malloc(sizeof(t_country) * (task->country_count + 1));
	fill_country(task->country, argv, task);
	init_map(task);
	get_day_count(task);

	int i;

	i = -1;
	while (++i < task->country_count)
		printf("%d\n", task->country[i].count_day);
	int x;
	int y;
	i = -1;
	while (++i < task->country_count)
	{
		y = -1;
		while (++y < task->height_map)
		{
			x = -1;
			while (++x < task->width_map)
			{
				printf("%ld	", task->map[y][x].count_money[i]);
			}
			printf("\n");
		}
		printf("\n");
	}
	printf("========\n");
}
