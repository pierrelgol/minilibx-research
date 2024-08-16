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
#include <assert.h>
#include <unistd.h>

#define HEIGHT 600
#define WIDTH 800

typedef struct s_player
{
	int32_t len;
	int32_t color;
	Vector2 pos;
	float_t velocity;

} t_player;

typedef struct s_data
{
	t_player player;
	int32_t  height;
	int32_t  width;
	void    *mlx;
	void    *win;
	void    *img;
	int32_t frame_rate;

	char   *pixel;
	int32_t bpp;
	int32_t line_size;
	int32_t endian;

} t_data;

void data_init(t_data *data)
{
	data->mlx = mlx_init();
	assert(data->mlx != NULL);

	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "mlx");
	assert(data->win != NULL);

	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	assert(data->img != NULL);

	data->pixel = mlx_get_data_addr(data->img, &data->bpp, &data->line_size, &data->endian);
	assert(data->pixel != NULL);

	printf("data->bpp = %d\n", data->bpp);
	printf("data->line_size = %d\n", data->line_size);
	printf("data->endian = %d\n", data->endian);
	fflush(stdout);
	data->player.len = 100;
	data->player.pos = (Vector2){.x = ((float) WIDTH / 2), .y = ((float) HEIGHT / 2)};
	data->player.color = 0xFFFF0000;
	data->player.velocity = 1.0f;
	data->frame_rate = 60;
}

int32_t data_deinit(void *param)
{
	t_data *data;

	data = (t_data *) param;
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_display(data->mlx);
	free(data->mlx);
	free(data);
	return (0);
}

void data_put_pixel(t_data *data, int32_t x, int32_t y, int32_t color)
{
	int32_t *pixel;

	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
	{
		pixel = ((int32_t *) &data->pixel[(y * 4) * WIDTH + (x * 4)]);
		*pixel = color;
	}
}

void data_clear(t_data *data, int32_t color)
{
	int32_t y;
	int32_t x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			data_put_pixel(data, x, y, color);
			++x;
		}
		++y;
	}
}

void draw_player(t_data *data, t_player *player)
{
	int32_t y;
	int32_t x;

	y = (int32_t) player->pos.y;
	while (y < ((int32_t) player->pos.y + player->len))
	{
		x = (int32_t) player->pos.x;
		while (x < ((int32_t) player->pos.x + player->len))
		{
			data_put_pixel(data, x, y, player->color);
			++x;
		}
		++y;
	}
}

void data_update(t_data *data)
{
	SetTargetFPS(data->frame_rate);
	data_clear(data, 0x00000000);
	draw_player(data, &data->player);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int32_t player_move(int32_t keycode, void *param)
{
	t_data *data;
	float	deltaTime;

	data = (t_data *) param;

	deltaTime = GetFrameTime();
	if (keycode == KEY_EQUAL)
		data->frame_rate += 1;
	if (keycode == KEY_MINUS)
		data->frame_rate -= 1;
	if (keycode == KEY_F)
		mlx_fps_show(data->mlx);
	if (keycode == KEY_G)
		mlx_fps_hide(data->mlx);
	if (keycode == KEY_W)
		data->player.pos.y -= (data->player.velocity * (1000.0f * deltaTime));
	if (keycode == KEY_S)
		data->player.pos.y += (data->player.velocity * (1000.0f * deltaTime));
	if (keycode == KEY_A)
		data->player.pos.x -= (data->player.velocity * (1000.0f * deltaTime));
	if (keycode == KEY_D)
		data->player.pos.x += (data->player.velocity * (1000.0f * deltaTime));
	if (keycode == KEY_ESCAPE)
		data_deinit(data);
	printf("player = (%.2f | %.2f) | ftime = %f\n", data->player.pos.x, data->player.pos.y, GetFrameTime());
	fflush(stdout);
	data_update(data);
	return (0);
}

int main(void)
{
	t_data *data = calloc(1, sizeof(t_data));

	data_init(data);

	mlx_fps_show(data->mlx);
	mlx_do_key_autorepeaton(data->mlx);
	mlx_key_hook(data->win, player_move, data);
	// mlx_hook(data->win, MLX_KEY_PRESSED, 0, player_move, data);
	mlx_hook(data->win, MLX_WINDOW_CLOSE, 0, data_deinit, data);
	mlx_loop(data->mlx);
	data_deinit(data);

	return (0);
}
