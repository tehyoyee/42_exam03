#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_zone
{
	int		width;
	int		height;
	char	background;
}			t_zone;

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;
}			t_shape;

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	close_all(FILE *file, char *drawing, char *msg)
{
	if (file)
		fclose(file);
	if (drawing)
		free(drawing);
	if (msg)
		write(1, msg, ft_strlen(msg));
	return (1);
}

char	*init_zone(FILE *file, t_zone *zone, char *drawing)
{
	int		i;

	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
		return (0);
	if (!(zone->width > 0 && zone->width <= 300 && zone->height > 0 && zone->height <= 300))
		return (0);
	if (!(drawing = (char *)malloc(sizeof(char) * (zone->width * zone->height))))
		return (0);
	i = 0;
	while (i < zone->width * zone->height)
		drawing[i++] = zone->background;
	return (drawing);
}

int	get_is_in(float x, float y, t_shape *shape)
{
	if (shape->x <= x && x <= shape->x + shape->width && shape->y <= y && y <= shape->y + shape->height)
	{
		if (x - shape->x < 1.0 || shape->x + shape->width - x < 1.0 || y - shape->y < 1.0 || shape->y + shape->height - y < 1.0)
			return (2);
		return (1);
	}
	return (0);
}

void	draw_shape(t_zone *zone, char *drawing, t_shape *shape)
{
	int	x;
	int	y;
	int	is_in;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			is_in = get_is_in((float)x, (float)y, shape);
			if ((is_in && shape->type == 'R') || (is_in == 2 && shape->type == 'r'))
				drawing[y * zone->width + x] = shape->color;
			x++;
		}
		y++;
	}
}

int	draw_shapes(FILE *file, t_zone *zone, char *drawing)
{
	t_shape	shape;
	int		check;

	while ((check = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.color)) == 6)
	{
		if (!((shape.type == 'r' || shape.type == 'R') && shape.width > 0 && shape.height > 0))
			return (0);
		draw_shape(zone, drawing, &shape);
	}

	if (check != -1)
		return (0);
	return (1);
}

void	draw_draw(t_zone *zone, char *drawing)
{
	int	i;

	i = 0;
	while (i < zone->height)
	{
		write(1, drawing + i * zone->width, zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_zone	zone;
	FILE 	*file;
	char	*drawing;

	drawing = NULL;
	if (argc != 2)
		return (close_all(NULL, NULL, "Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (close_all(NULL, NULL, "Error: Operation file corrupted\n"));
	if (!(drawing = init_zone(file, &zone, drawing)))
		return (close_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!(draw_shapes(file, &zone, drawing)))
		return (close_all(file, drawing, "Error: Operation file corrupted\n"));
	draw_draw(&zone, drawing);
	close_all(file, drawing, NULL);
}
