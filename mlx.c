/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:41:19 by pollivie          #+#    #+#             */
/*   Updated: 2024/08/14 12:41:19 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include <stdio.h>
#include <string.h>

static int32_t empty_mouse_move_hook(int32_t mouse_x, int32_t mouse_y, void *argument)
{
	IGNORE_ARGUMENT(mouse_x);
	IGNORE_ARGUMENT(mouse_y);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t empty_mouse_pressed_hook(int32_t button, void *argument)
{
	IGNORE_ARGUMENT(button);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t empty_mouse_released_hook(int32_t button, void *argument)
{
	IGNORE_ARGUMENT(button);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t empty_key_pressed_hook(int32_t keycode, void *argument)
{
	IGNORE_ARGUMENT(keycode);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t empty_key_released_hook(int32_t keycode, void *argument)
{
	IGNORE_ARGUMENT(keycode);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t empty_loop_hook_begin(void *argument)
{
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t empty_loop_hook_ends(void *argument)
{
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t empty_window_resized_hook(int32_t width, int32_t height, void *argument)
{
	IGNORE_ARGUMENT(width);
	IGNORE_ARGUMENT(height);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t empty_window_event_hook(int32_t event, void *argument)
{
	IGNORE_ARGUMENT(event);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static void mlx_instance_setup_hooks(t_mlx_instance *instance)
{
	instance->mlx_even_list[EVENT_NO_EVENT] = (t_mlx_hooks){0};
	instance->mlx_even_list[EVENT_MOUSE_MOVE] = (t_mlx_hooks){.mouse_move = empty_mouse_move_hook};
	instance->mlx_even_list[EVENT_MOUSE_PRESSED] = (t_mlx_hooks){.mouse_pressed = empty_mouse_pressed_hook};
	instance->mlx_even_list[EVENT_MOUSE_RELEASED] = (t_mlx_hooks){.mouse_released = empty_mouse_released_hook};
	instance->mlx_even_list[EVENT_KEY_PRESSED] = (t_mlx_hooks){.key_pressed = empty_key_pressed_hook};
	instance->mlx_even_list[EVENT_KEY_RELEASED] = (t_mlx_hooks){.key_released = empty_key_released_hook};
	instance->mlx_even_list[EVENT_LOOP_BEGINS] = (t_mlx_hooks){.loop_begins = empty_loop_hook_begin};
	instance->mlx_even_list[EVENT_LOOP_ENDS] = (t_mlx_hooks){.loop_ends = empty_loop_hook_ends};
	instance->mlx_even_list[EVENT_WINDOW_RESIZED] = (t_mlx_hooks){.window_resized = empty_window_resized_hook};
	instance->mlx_even_list[EVENT_WINDOW_GENERIC] = (t_mlx_hooks){.window_generic = empty_window_event_hook};
}

void *mlx_instance_create(void)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) calloc(1, sizeof(t_mlx_instance));
	if (!instance)
		return (NULL);
	instance->active_window = false;
	instance->color_format = MLX_COLOR_FORMAT;
	mlx_instance_setup_hooks(instance);
	TraceLog(LOG_INFO, "creating mlx instance");
	return ((void *) instance);
}

void mlx_instance_destroy(void *mlx_ptr)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	mlx_image_destroy(mlx_ptr, instance->image);
	free(instance);
	TraceLog(LOG_INFO, "Destroying mlx instance");
}

void *mlx_window_create(void *mlx_ptr, int32_t width, int32_t height, char *title)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	instance = (t_mlx_instance *) mlx_ptr;
	if (instance->window != NULL)
		return (NULL);
	window = (t_mlx_window *) calloc(1, sizeof(t_mlx_window));
	if (!window)
		return (NULL);
	InitWindow(width, height, title);
	window->height = height;
	window->width = width;
	window->image = NULL;
	window->title = title;
	instance->window = window;
	instance->image = mlx_image_create(mlx_ptr, width, height);
	UpdateTexture(instance->image->texture, instance->image->buffer);
	instance->active_image = true;
	TraceLog(LOG_INFO, "creating window");
	return ((void *) window);
}

void mlx_window_destroy(void *mlx_ptr, void *win_ptr)
{
	t_mlx_instance *instance;
	t_mlx_window   *window;

	instance = (t_mlx_instance *) mlx_ptr;
	window = (t_mlx_window *) win_ptr;
	TraceLog(LOG_INFO, "destroying window");
	free(window);
	instance->window = NULL;
	instance->active_window = false;
	CloseWindow();
}

void *mlx_image_create(void *mlx_ptr, int32_t width, int32_t height)
{
	t_mlx_instance *instance;
	t_mlx_image    *image;

	instance = (t_mlx_instance *) mlx_ptr;
	image = (t_mlx_image *) calloc(1, sizeof(t_mlx_image));
	if (!image)
	{
		TraceLog(LOG_ERROR, "allocation failure image");
		return (NULL);
	}

	image->buffer = (uint8_t *) calloc(height * width, sizeof(uint32_t));
	if (!image->buffer)
	{
		TraceLog(LOG_ERROR, "allocation failure image buffer");
		free(image);
		return (NULL);
	}
	else
	{
		image->format = instance->color_format;
		image->mimaps = 1;
		image->height = height;
		image->width = width;
		image->texture = LoadTextureFromImage((Image){
		    .mipmaps = image->mimaps,
		    .height = image->height,
		    .format = image->format,
		    .width = image->width,
		    .data = image->buffer,
		});
		image->is_loaded = true;
		TraceLog(LOG_INFO, "creating image");
		return ((void *) image);
	}
}

void mlx_image_destroy(void *mlx_ptr, void *img_ptr)
{
	t_mlx_instance *instance;
	t_mlx_image    *image;

	instance = (t_mlx_instance *) mlx_ptr;
	image = (t_mlx_image *) img_ptr;
	TraceLog(LOG_INFO, "destroying image");
	if (image->is_loaded)
	{
		UnloadTexture(image->texture);
		image->is_loaded = false;
	}
	free(image->buffer);
	free(image);
	(void) instance;
}

char *mlx_image_buffer(void *img_ptr, int32_t *img_width, int32_t *img_height, int32_t *img_bpp)
{
	t_mlx_image *image;

	image = (t_mlx_image *) img_ptr;
	*img_width = image->width;
	*img_height = image->height;
	*img_bpp = sizeof(uint32_t);
	return ((char *) image->buffer);
}

void mlx_window_put_image(void *win_ptr, void *img_ptr, int32_t x, int32_t y)
{
	t_mlx_window *window;
	t_mlx_image  *image;

	window = (t_mlx_window *) win_ptr;
	image = (t_mlx_image *) img_ptr;
	window->texture = image->texture;
	UpdateTexture(image->texture, image->buffer);
	BeginDrawing();
	ClearBackground(BLACK);
	DrawFPS(20, 20);
	DrawTexture(image->texture, x, y, WHITE);
	EndDrawing();
}

void mlx_do_sync(void *mlx_ptr)
{
	t_mlx_instance *instance;

	instance = (t_mlx_instance *) mlx_ptr;
	BeginDrawing();
	(void) instance;
	ClearBackground(BLACK);
	DrawFPS(20, 20);
	DrawTexture(instance->window->texture, 0, 0, WHITE);
	EndDrawing();
}

void mlx_image_put_pixel(void *mlx_ptr, int32_t x, int32_t y, int32_t color)
{
	t_mlx_image *image;
	int32_t     *ptr;

	image = (t_mlx_image *) mlx_ptr;
	if (x >= 0 && y >= 0 && x < image->width && y < image->height)
	{
		ptr = (int32_t *) (&image->buffer[(y * image->width * 4) + (x * 4)]);
		*ptr = color;
	}
}

void mlx_instance_loop(void *mlx_ptr)
{
	t_mlx_instance *instance;
	int32_t         keycode;
	t_mlx_hooks     hook;

	instance = (t_mlx_instance *) mlx_ptr;
	SetTargetFPS(120);
	// SetExitKey(0);
	while (!WindowShouldClose())
	{
		keycode = GetKeyPressed();
		hook = instance->mlx_even_list[EVENT_KEY_PRESSED];
		while (keycode && !IsKeyReleased(keycode))
			hook.key_pressed(keycode, hook.argument);
		mlx_do_sync(instance);
	}
}

void mlx_key_hooks(void *mlx_ptr, int32_t (*key_hook_fn)(int32_t keycode, void *argument), void *argument)
{
	t_mlx_instance *instance;
	t_mlx_hooks    *hooks;

	instance = (t_mlx_instance *) mlx_ptr;
	hooks = &instance->mlx_even_list[EVENT_KEY_PRESSED];
	hooks->key_pressed = key_hook_fn;
	hooks->argument = argument;
	hooks->kind = EVENT_KEY_PRESSED;
}
