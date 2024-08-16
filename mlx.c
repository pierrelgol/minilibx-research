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

int mlx_loop(void *mlx_ptr)
{
	t_mlx_instance *instance;
	t_mlx_hooks    *hooks;
	t_mlx_hooks     hook;

	bool has_keyboard_subscribers;
	bool has_generic_subscribers;
	bool has_window_subscribers;
	bool has_mouse_subscribers;

	instance = (t_mlx_instance *) mlx_ptr;
	has_keyboard_subscribers = _mlx_keyboard_event_has_subscribers(instance);
	has_generic_subscribers = _mlx_generic_event_has_subscribers(instance);
	has_window_subscribers = _mlx_window_event_has_subscribers(instance);
	has_mouse_subscribers = _mlx_mouse_event_has_subscribers(instance);

	while (!WindowShouldClose() && instance->loop_ends)
	{
		if (has_keyboard_subscribers)
			_mlx_try_broadcast_keyboard_event(instance, instance->mlx_even_list);
		if (has_generic_subscribers)
			_mlx_try_broadcast_generic_event(instance, instance->mlx_even_list);
		if (has_window_subscribers)
			_mlx_try_broadcast_window_event(instance, instance->mlx_even_list);
		if (has_mouse_subscribers)
			_mlx_try_broadcast_mouse_event(instance, instance->mlx_even_list);
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

int mlx_do_sync(void *mlx_ptr)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;
	t_mlx_image    *image;

	instance = (t_mlx_instance *) mlx_ptr;
	window = instance->window;
	image = instance->_image;
	mlx_put_image_to_window(instance, window, image, 0, 0);
	return (0);
}

int mlx_get_screen_size(void *mlx_ptr, int *sizex, int *sizey)
{
	int32_t monitor;

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
	*(image->active_buffer) = false;
	*(image->active_texture) = false;
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
	instance->font = GetFontDefault();
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
	window->active_image = false;
	window->active_texture = false;
	instance->window = NULL;
	free(window);
	CloseWindow();
	return (0);
}

int mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y)
{
	t_mlx_window *window;

	window = (t_mlx_window *) win_ptr;
	_mlx_window_update_buffer(mlx_ptr, win_ptr, img_ptr);
	UpdateTexture(window->texture, window->image.data);
	BeginDrawing();
	ClearBackground(BLACK);
	DrawTexture(window->texture, x, y, WHITE);
	EndDrawing();
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
		instance->window = window;
		window->_parent = mlx_ptr;
		window->title = title;
		window->width = size_x;
		window->height = size_y;
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
		window->image = (Image){
		    .width = instance->_image->width,
		    .height = instance->_image->height,
		    .data = instance->_image->buffer,
		    .format = instance->_image->format,
		    .mipmaps = instance->_image->mimaps,
		};
		window->texture = instance->_image->texture;
		window->active_image = true;
		window->active_texture = true;
		instance->_image->active_buffer = &window->active_image;
		instance->_image->active_texture = &window->active_image;
		InitWindow(window->width, window->height, window->title);
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
		image->active_buffer = NULL;
		image->active_texture = NULL;
		image->format = instance->format;
		image->height = height;
		image->width = width;
		image->texture = LoadTextureFromImage((Image){
		    .mipmaps = image->mimaps,
		    .height = image->height,
		    .format = image->format,
		    .width = image->width,
		    .data = image->buffer,
		});
	}
	return ((void *) image);
}

char *mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian)
{
	t_mlx_image *image;

	image = (t_mlx_image *) img_ptr;

	*bits_per_pixel = (sizeof(uint32_t) * 8);
	*size_line = (image->width * sizeof(uint32_t));
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
	if (image->active_buffer != NULL)
		*(image->active_buffer) = false;
	if (image->active_texture != NULL)
		*(image->active_texture) = false;
	UnloadTexture(image->texture);
	free(image->buffer);
	free(image);
	return (0);
}

int mlx_pixel_put(void *mlx_ptr, void *win_ptr, int x, int y, int color)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;
	int32_t         y_offset;
	int32_t         x_offset;

	instance = (t_mlx_instance *) mlx_ptr;
	window = (t_mlx_window *) win_ptr;
	_mlx_window_update_buffer(instance, window, instance->_image);
	y_offset = (y * sizeof(uint32_t) * window->image.width);
	x_offset = (x * sizeof(uint32_t));
	*((int32_t *) window->image.data + (y_offset + x_offset)) = color;
	mlx_put_image_to_window(instance, window, instance->_image, 0, 0);
	return (0);
}

int mlx_mouse_hook(void *win_ptr, int (*funct_ptr)(), void *param)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	window = (t_mlx_window *) win_ptr;
	instance = window->_parent;
	instance->mlx_even_list[EVENT_GENERIC_MOUSE].kind = EVENT_GENERIC_MOUSE;
	instance->mlx_even_list[EVENT_GENERIC_MOUSE].argument = param;
	instance->mlx_even_list[EVENT_GENERIC_MOUSE].generic_mouse = funct_ptr;
	instance->mlx_even_list[EVENT_GENERIC_MOUSE].active = true;
	return (0);
}

int mlx_key_hook(void *win_ptr, int (*funct_ptr)(), void *param)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	window = (t_mlx_window *) win_ptr;
	instance = window->_parent;
	instance->mlx_even_list[EVENT_GENERIC_KEYBOARD].kind = EVENT_GENERIC_KEYBOARD;
	instance->mlx_even_list[EVENT_GENERIC_KEYBOARD].argument = param;
	instance->mlx_even_list[EVENT_GENERIC_KEYBOARD].generic_keyboard = funct_ptr;
	instance->mlx_even_list[EVENT_GENERIC_KEYBOARD].active = true;
	return (0);
}

int mlx_expose_hook(void *win_ptr, int (*funct_ptr)(), void *param)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	window = (t_mlx_window *) win_ptr;
	instance = window->_parent;
	instance->mlx_even_list[EVENT_GENERIC_WINDOW].kind = EVENT_GENERIC_WINDOW;
	instance->mlx_even_list[EVENT_GENERIC_WINDOW].argument = param;
	instance->mlx_even_list[EVENT_GENERIC_WINDOW].generic_window = funct_ptr;
	instance->mlx_even_list[EVENT_GENERIC_WINDOW].active = true;
	return (0);
}

int mlx_loop_hook(void *mlx_ptr, int (*funct_ptr)(), void *param)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	instance->mlx_even_list[EVENT_LOOP_BEGINS].kind = EVENT_LOOP_BEGINS;
	instance->mlx_even_list[EVENT_LOOP_BEGINS].argument = param;
	instance->mlx_even_list[EVENT_LOOP_BEGINS].loop_begins = funct_ptr;
	instance->mlx_even_list[EVENT_LOOP_BEGINS].active = true;

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

	instance = (t_mlx_instance *) instance;
	DrawTextEx(instance->font, string, (Vector2){.x = x, .y = y}, instance->font_size, 1.0f, GetColor(color));
	return (0);
}

void mlx_set_font(void *mlx_ptr, void *win_ptr, char *name)
{
	t_mlx_instance *instance;

	IGNORE_ARGUMENT(win_ptr);
	instance = (t_mlx_instance *) instance;
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

	IGNORE_ARGUMENT(x_mask);
	window = (t_mlx_window *) win_ptr;
	instance = window->_parent;
	instance->mlx_even_list[x_event].kind = x_event;
	instance->mlx_even_list[x_event].argument = param;
	instance->mlx_even_list[x_event].generic_window = funct;
	instance->mlx_even_list[x_event].active = true;
	return (0);
}

int mlx_do_key_autorepeatoff(void *mlx_ptr)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	instance->autorepeat = 0;
	return (0);
}

int mlx_do_key_autorepeaton(void *mlx_ptr)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	instance->autorepeat = 1;
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
