#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
	float	r;
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

char	*draw_zone(FILE *file, t_zone *zone, char *drawing)
{
	int	i;

	if ((fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (NULL);
	if (!(0 < zone->width && zone->width <= 300 && 0 < zone->height && zone->height <= 300))
		return (NULL);
	if (!(drawing = (char *)malloc(sizeof(char) * (zone->width * zone->height))))
		return (NULL);
	i = 0;
	while (i < zone->width * zone->height)
		drawing[i++] = zone->background;
	return (drawing);
}

int	get_is_in(float x, float y, t_shape *shape)
{
	float	distance;

	distance = sqrtf(powf(x - shape->x, 2) + powf(y - shape->y, 2));
	if (distance <= shape->r)
	{
		if (shape->r - distance < 1.0)
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
			if ((is_in && shape->type == 'C') || (is_in == 2 && shape->type == 'c'))
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

	while ((check = fscanf(file, "%c %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.r, &shape.color)) == 5)
	{
		if (!(0 < shape.r && (shape.type == 'c' || shape.type == 'C')))
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
	FILE	*file;
	char	*drawing;

	drawing = NULL;
	if (argc != 2)
		return (close_all(NULL, NULL, "Error: argument\n"));
	if (!(file = fopen(argv[1], "r")))
		return (close_all(NULL, NULL, "Error: Operation file corrupted\n"));
	if (!(drawing = draw_zone(file, &zone, drawing)))
		return (close_all(file, NULL, "Error: Operation file corrupted\n"));
	if (!draw_shapes(file, &zone, drawing))
		return (close_all(file, drawing, "Error: Operation file corrupted\n"));
	draw_draw(&zone, drawing);
	close_all(file, drawing, NULL);
}