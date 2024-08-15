/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:19:31 by pollivie          #+#    #+#             */
/*   Updated: 2024/08/15 11:19:32 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <unistd.h>

typedef struct s_data
{
	void   *mlx_ptr;
	void   *win_ptr;
	void   *img_ptr;
	int32_t x;
	int32_t y;

} t_data;

void draw_clear(t_mlx_image *image, uint32_t color)
{
	int32_t y;
	int32_t x;

	y = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
			mlx_image_put_pixel(image, x++, y, color);
		++y;
	}
}

void draw_cube(t_mlx_image *image, int32_t x_start, int32_t y_start, int32_t length, int32_t color)
{
	int32_t y;
	int32_t x;

	y = y_start;
	while (y < y_start + length)
	{
		x = x_start;
		while (x < x_start + length)
			mlx_image_put_pixel(image, x++, y, color);
		++y;
	}
}

void draw(void *win_ptr, void *img_ptr, int32_t x, int32_t y)
{
	draw_clear(img_ptr, 0x00000000);
	draw_cube(img_ptr, x, y, 100, 0xFFFF0000);
	mlx_window_put_image(win_ptr, img_ptr, 0, 0);
}

int32_t on_keypressed(int32_t keycode, void *argument)
{
	t_data *data;

	data = argument;

	if (keycode == KEY_W)
	{
		data->y -= 5;
	}
	else if (keycode == KEY_S)
	{
		data->y += 5;
	}
	else if (keycode == KEY_A)
	{
		data->x -= 5;
	}
	else if (keycode == KEY_D)
	{
		data->x += 5;
	}
	draw(data->win_ptr, data->img_ptr, data->x, data->y);
	return (0);
}

int main(void)
{
	t_data *data = calloc(1, sizeof(t_data));
	void   *mlx_ptr;
	void   *win_ptr;
	void   *img_ptr;

	mlx_ptr = mlx_instance_create();
	win_ptr = mlx_window_create(mlx_ptr, 800, 600, "Hello World!");
	img_ptr = mlx_image_create(mlx_ptr, 800, 600);

	data->x = 0;
	data->y = 0;
	data->mlx_ptr = mlx_ptr;
	data->win_ptr = win_ptr;
	data->img_ptr = img_ptr;

	mlx_key_hooks(mlx_ptr, on_keypressed, data);
	mlx_instance_loop(mlx_ptr);

	mlx_image_destroy(mlx_ptr, img_ptr);
	mlx_window_destroy(mlx_ptr, win_ptr);
	mlx_instance_destroy(mlx_ptr);
	free(data);

	return (0);
}
