/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:33:49 by pollivie          #+#    #+#             */
/*   Updated: 2024/08/16 11:33:49 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"

int mlx_do_sync(void *mlx_ptr)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	instance = (t_mlx_instance *) mlx_ptr;
	window = instance->window;
	UpdateTexture(window->texture, window->image.data);
	BeginDrawing();
	BeginBlendMode(BLEND_ADDITIVE);
	ClearBackground(BLACK);
	if (instance->print_fps)
		DrawFPS(20, 20);
	DrawTexture(window->texture, 0, 0, WHITE);
	EndBlendMode();
	EndDrawing();
	return (0);
}

int mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	instance = (t_mlx_instance *) mlx_ptr;
	window = (t_mlx_window *) win_ptr;
	if (img_ptr != NULL)
	{
		window->image = ((t_mlx_image *) img_ptr)->image;
		window->texture = ((t_mlx_image *) img_ptr)->texture;
	}
	else
	{
		window->image = instance->_image->image;
		window->texture = instance->_image->texture;
	}
	UpdateTexture(window->texture, window->image.data);
	BeginDrawing();
	BeginBlendMode(BLEND_ADDITIVE);
	ClearBackground(BLACK);
	if (instance->print_fps)
		DrawFPS(20, 20);
	DrawTexture(window->texture, x, y, WHITE);
	EndBlendMode();
	EndDrawing();
	return (0);
}

int mlx_loop(void *mlx_ptr)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	while (!WindowShouldClose() && !instance->loop_ends)
	{
		if (_mlx_should_broadcast_key_event(instance) && _mlx_broadcast_keyboard_event(instance))
			continue;
		mlx_do_sync(instance);
	}
	return (0);
}

int mlx_loop_end(void *mlx_ptr)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	instance->loop_ends = 1;
	return (0);
}


int mlx_get_screen_size(void *mlx_ptr, int *sizex, int *sizey)
{
	int32_t monitor;

	IGNORE_ARGUMENT(mlx_ptr);
	monitor = GetCurrentMonitor();
	*sizey = GetMonitorPhysicalHeight(monitor);
	*sizex = GetMonitorPhysicalWidth(monitor);
	return (0);
}

int mlx_destroy_display(void *mlx_ptr)
{
	t_mlx_instance *instance;
	t_mlx_image    *image;

	instance = (t_mlx_instance *) mlx_ptr;
	image = (t_mlx_image *) instance->_image;
	free(image->buffer);
	free(image);
	instance->_image = NULL;
	return (0);
}

void *mlx_init(void)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) calloc(1, sizeof(t_mlx_instance));
	if (!instance)
	{
		return (NULL);
	}
	_mlx_init_event_list(instance);
	instance->autorepeat = 0;
	instance->format = MLX_COLOR_FORMAT;
	instance->window = NULL;
	instance->_image = NULL;
	instance->loop_ends = 0;
	return ((void *) instance);
}

int mlx_clear_window(void *mlx_ptr, void *win_ptr)
{
	IGNORE_ARGUMENT(mlx_ptr);
	IGNORE_ARGUMENT(win_ptr);
	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing();
	return (0);
}

int mlx_destroy_window(void *mlx_ptr, void *win_ptr)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	instance = (t_mlx_instance *) mlx_ptr;
	window = (t_mlx_window *) win_ptr;
	window->_parent = NULL;
	instance->window = NULL;
	free(window);
	CloseWindow();
	return (0);
}


void *mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	instance = (t_mlx_instance *) mlx_ptr;
	if (instance->window)
	{
		mlx_destroy_window(instance, instance->window);
	}
	window = (t_mlx_window *) calloc(1, sizeof(t_mlx_window));
	if (!window)
	{
		return (NULL);
	}
	else
	{
		window->width = size_x;
		window->height = size_y;
		instance->window = window;
		window->_parent = mlx_ptr;
		window->title = title;
		InitWindow(window->width, window->height, window->title);
		if (instance->_image == NULL)
		{
			instance->_image = mlx_new_image(instance, size_x, size_y);
			if (!instance->_image)
			{
				instance->window = NULL;
				free(window);
				return (NULL);
			}
		}
		window->image = instance->_image->image;
		window->texture = instance->_image->texture;
	}
	return ((void *) window);
}

void *mlx_new_image(void *mlx_ptr, int width, int height)
{
	t_mlx_instance *instance;
	t_mlx_image    *image;

	instance = (t_mlx_instance *) mlx_ptr;
	image = (t_mlx_image *) calloc(1, sizeof(t_mlx_image));
	if (!image)
	{
		return (NULL);
	}
	image->buffer = (char *) calloc(width * height, sizeof(uint32_t));
	if (!image->buffer)
	{
		free(image);
		return (NULL);
	}
	else
	{
		image->format = instance->format;
		image->height = height;
		image->width = width;
		image->mimaps = 1;
		image->image = (Image){
			.data = image->buffer,
			.format = instance->format,
			.height = image->height,
			.width = image->width,
			.mipmaps = image->mimaps,
		};
		image->texture = LoadTextureFromImage(image->image);
		image->is_loaded = true;
	}
	return ((void *) image);
}

char *mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian)
{
	t_mlx_image *image;

	image = (t_mlx_image *) img_ptr;

	*bits_per_pixel = (sizeof(uint32_t) * 8);
	*size_line = image->width;
	*endian = (0);
	return (image->buffer);
}

int mlx_destroy_image(void *mlx_ptr, void *img_ptr)
{
	t_mlx_instance *instance;
	t_mlx_image    *image;

	instance = (t_mlx_instance *) mlx_ptr;
	image = (t_mlx_image *) img_ptr;
	if (image == instance->_image)
		instance->_image = NULL;
	UnloadTexture(image->texture);
	image->is_loaded = false;
	free(image->buffer);
	free(image);
	return (0);
}

int mlx_pixel_put(void *mlx_ptr, void *win_ptr, int x, int y, int color)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;
	t_mlx_image    *image;
	int32_t         y_offset;
	int32_t         x_offset;

	instance = (t_mlx_instance *) mlx_ptr;
	window = (t_mlx_window *) win_ptr;
	image = instance->_image;
	y_offset = (y * sizeof(uint32_t) * image->width);
	x_offset = (x * sizeof(uint32_t));
	*((int32_t *) image->buffer + (y_offset + x_offset)) = color;
	mlx_put_image_to_window(instance, window, instance->_image, 0, 0);
	return (0);
}

int mlx_mouse_hook(void *win_ptr, int (*funct_ptr)(), void *param)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	window = (t_mlx_window *) win_ptr;
	instance = window->_parent;
	instance->mlx_even_list[MLX_GENERIC_MOUSE_HOOK].kind = MLX_GENERIC_MOUSE_HOOK;
	instance->mlx_even_list[MLX_GENERIC_MOUSE_HOOK].argument = param;
	instance->mlx_even_list[MLX_GENERIC_MOUSE_HOOK].generic_mouse_hook = funct_ptr;
	instance->mlx_even_list[MLX_GENERIC_MOUSE_HOOK].active = true;
	return (0);
}

int mlx_key_hook(void *win_ptr, int (*funct_ptr)(), void *param)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	window = (t_mlx_window *) win_ptr;
	instance = window->_parent;
	instance->mlx_even_list[MLX_GENERIC_KEY_HOOK].kind = MLX_GENERIC_KEY_HOOK;
	instance->mlx_even_list[MLX_GENERIC_KEY_HOOK].argument = param;
	instance->mlx_even_list[MLX_GENERIC_KEY_HOOK].generic_key_hook = funct_ptr;
	instance->mlx_even_list[MLX_GENERIC_KEY_HOOK].active = true;
	return (0);
}

int mlx_expose_hook(void *win_ptr, int (*funct_ptr)(), void *param)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	window = (t_mlx_window *) win_ptr;
	instance = window->_parent;
	instance->mlx_even_list[MLX_GENERIC_WINDOW_HOOK].kind = MLX_GENERIC_WINDOW_HOOK;
	instance->mlx_even_list[MLX_GENERIC_WINDOW_HOOK].argument = param;
	instance->mlx_even_list[MLX_GENERIC_WINDOW_HOOK].generic_window_hook = funct_ptr;
	instance->mlx_even_list[MLX_GENERIC_WINDOW_HOOK].active = true;
	return (0);
}

int mlx_loop_hook(void *mlx_ptr, int (*funct_ptr)(), void *param)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	instance->mlx_even_list[MLX_LOOP_HOOK].kind = MLX_LOOP_HOOK;
	instance->mlx_even_list[MLX_LOOP_HOOK].argument = param;
	instance->mlx_even_list[MLX_LOOP_HOOK].loop_hook = funct_ptr;
	instance->mlx_even_list[MLX_LOOP_HOOK].active = true;

	return (0);
}

int mlx_get_color_value(void *mlx_ptr, int color)
{
	Color result;

	IGNORE_ARGUMENT(mlx_ptr);
	result.a = (color >> 24) & 0xFF000000;
	result.r = (color >> 16) & 0x00FF0000;
	result.g = (color >> 8) & 0x0000FF00;
	result.b = (color) & 0x000000FF;
	return (ColorToInt(result));
}

int mlx_string_put(void *mlx_ptr, void *win_ptr, int x, int y, int color, char *string)
{
	t_mlx_instance *instance;

	IGNORE_ARGUMENT(win_ptr);
	instance = (t_mlx_instance *) mlx_ptr;
	DrawTextEx(instance->font, string, (Vector2){.x = x, .y = y}, instance->font_size, 1.0f, GetColor(color));
	return (0);
}

void mlx_set_font(void *mlx_ptr, void *win_ptr, char *name)
{
	t_mlx_instance *instance;

	IGNORE_ARGUMENT(win_ptr);
	instance = (t_mlx_instance *) mlx_ptr;
	UnloadFont(instance->font);
	instance->font = LoadFont(name);
}

void *mlx_xpm_to_image(void *mlx_ptr, char **xpm_data, int *width, int *height)
{
	IGNORE_ARGUMENT(mlx_ptr);
	IGNORE_ARGUMENT(xpm_data);
	IGNORE_ARGUMENT(width);
	IGNORE_ARGUMENT(height);
	return (NULL);
}

void *mlx_xpm_file_to_image(void *mlx_ptr, char *filename, int *width, int *height)
{
	IGNORE_ARGUMENT(mlx_ptr);
	IGNORE_ARGUMENT(filename);
	IGNORE_ARGUMENT(width);
	IGNORE_ARGUMENT(height);
	return (NULL);
}

int mlx_hook(void *win_ptr, int x_event, int x_mask, int (*funct)(), void *param)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;
	t_mlx_hooks    *hook;

	IGNORE_ARGUMENT(x_mask);
	window = (t_mlx_window *) win_ptr;
	instance = window->_parent;
	hook = &instance->mlx_even_list[MLX_MAX_EVENT - 1];
	if (x_event == MLX_KEY_PRESSED)
		hook = &instance->mlx_even_list[MLX_KEY_PRESSED];
	else if (x_event == MLX_KEY_RELEASED)
		hook = &instance->mlx_even_list[MLX_KEY_RELEASED];
	else if (x_event == MLX_BUTTON_PRESSED)
		hook = &instance->mlx_even_list[MLX_BUTTON_PRESSED];
	else if (x_event == MLX_BUTTON_RELEASED)
		hook = &instance->mlx_even_list[MLX_BUTTON_RELEASED];
	else if (x_event == MLX_MOUVE_MOVE)
		hook = &instance->mlx_even_list[MLX_MOUVE_MOVE];
	else if (x_event == MLX_WINDOW_CLOSE)
		hook = &instance->mlx_even_list[MLX_WINDOW_CLOSE];
	else if (x_event == MLX_WINDOW_FOCUS_IN)
		hook = &instance->mlx_even_list[MLX_WINDOW_FOCUS_IN];
	else if (x_event == MLX_WINDOW_FOCUS_OUT)
		hook = &instance->mlx_even_list[MLX_WINDOW_FOCUS_OUT];
	else if (x_event == MLX_WINDOW_ENTER)
		hook = &instance->mlx_even_list[MLX_WINDOW_ENTER];
	else if (x_event == MLX_WINDOW_LEAVE)
		hook = &instance->mlx_even_list[MLX_WINDOW_LEAVE];
	else if (x_event == MLX_WINDOW_RESIZE)
		hook = &instance->mlx_even_list[MLX_WINDOW_RESIZE];
	else if (x_event == MLX_LOOP_HOOK)
		hook = &instance->mlx_even_list[MLX_LOOP_HOOK];
	hook->active = true;
	hook->argument = param;
	hook->generic_hook = funct;
	return (0);
}

int mlx_do_key_autorepeatoff(void *mlx_ptr)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	instance->autorepeat = false;
	return (0);
}

int mlx_do_key_autorepeaton(void *mlx_ptr)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	instance->autorepeat = true;
	return (0);
}

int mlx_mouse_get_pos(void *mlx_ptr, void *win_ptr, int *x, int *y)
{
	IGNORE_ARGUMENT(mlx_ptr);
	IGNORE_ARGUMENT(win_ptr);

	*x = GetMouseX();
	*y = GetMouseY();
	return (0);
}

int mlx_mouse_move(void *mlx_ptr, void *win_ptr, int x, int y)
{
	IGNORE_ARGUMENT(mlx_ptr);
	IGNORE_ARGUMENT(win_ptr);
	SetMousePosition(x, y);
	return (0);
}

int mlx_mouse_hide(void *mlx_ptr, void *win_ptr)
{
	IGNORE_ARGUMENT(mlx_ptr);
	IGNORE_ARGUMENT(win_ptr);
	HideCursor();
	return (0);
}

int mlx_mouse_show(void *mlx_ptr, void *win_ptr)
{
	IGNORE_ARGUMENT(mlx_ptr);
	IGNORE_ARGUMENT(win_ptr);
	ShowCursor();
	return (0);
}

int mlx_fps_show(void *mlx_ptr)
{
	t_mlx_instance	*instance;

	instance = (t_mlx_instance*)mlx_ptr;
	instance->print_fps = true;
	return (0);
}

int mlx_fps_hide(void *mlx_ptr)
{
	t_mlx_instance	*instance;

	instance = (t_mlx_instance*)mlx_ptr;
	instance->print_fps = false;
	return (0);
}

