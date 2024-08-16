/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_int.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 11:36:31 by pollivie          #+#    #+#             */
/*   Updated: 2024/08/16 11:36:32 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_int.h"

static int32_t _mlx_mouse_move_hook(int32_t mouse_x, int32_t mouse_y, void *argument)
{
	IGNORE_ARGUMENT(mouse_x);
	IGNORE_ARGUMENT(mouse_y);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t _mlx_mouse_pressed_hook(int32_t button, void *argument)
{
	IGNORE_ARGUMENT(button);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t _mlx_mouse_released_hook(int32_t button, void *argument)
{
	IGNORE_ARGUMENT(button);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t _mlx_key_pressed_hook(int32_t keycode, void *argument)
{
	IGNORE_ARGUMENT(keycode);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t _mlx_key_released_hook(int32_t keycode, void *argument)
{
	IGNORE_ARGUMENT(keycode);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t _mlx_loop_hook_begins(void *argument)
{
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t _mlx_loop_hook_ends(void *argument)
{
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t _mlx_windows_resized_hook(int32_t width, int32_t height, void *argument)
{
	IGNORE_ARGUMENT(width);
	IGNORE_ARGUMENT(height);
	IGNORE_ARGUMENT(argument);
	return (0);
}

static int32_t _mlx_windows_event_hook(int32_t event, void *argument)
{
	IGNORE_ARGUMENT(event);
	IGNORE_ARGUMENT(argument);
	return (0);
}

void _mlx_init_event_list(t_mlx_instance *const instance)
{
	instance->mlx_even_list[EVENT_NO_EVENT] = (t_mlx_hooks){0};
	instance->mlx_even_list[EVENT_GENERIC_KEYBOARD] = (t_mlx_hooks){
	    .generic_keyboard = _mlx_key_pressed_hook,
	};
	instance->mlx_even_list[EVENT_GENERIC_MOUSE] = (t_mlx_hooks){
	    .generic_mouse = _mlx_mouse_move_hook,
	};
	instance->mlx_even_list[EVENT_GENERIC_WINDOW] = (t_mlx_hooks){
	    .window_generic = _mlx_windows_event_hook,
	};
	instance->mlx_even_list[EVENT_MOUSE_MOVE] = (t_mlx_hooks){.mouse_move = _mlx_mouse_move_hook, .active = false};
	instance->mlx_even_list[EVENT_MOUSE_PRESSED] = (t_mlx_hooks){.mouse_pressed = _mlx_mouse_pressed_hook, .active = false};
	instance->mlx_even_list[EVENT_MOUSE_RELEASED] = (t_mlx_hooks){.mouse_released = _mlx_mouse_released_hook, .active = false};
	instance->mlx_even_list[EVENT_KEY_PRESSED] = (t_mlx_hooks){.key_pressed = _mlx_key_pressed_hook, .active = false};
	instance->mlx_even_list[EVENT_KEY_RELEASED] = (t_mlx_hooks){.key_released = _mlx_key_released_hook, .active = false};
	instance->mlx_even_list[EVENT_LOOP_BEGINS] = (t_mlx_hooks){.loop_begins = _mlx_loop_hook_begins, .active = false};
	instance->mlx_even_list[EVENT_WINDOW_RESIZED] = (t_mlx_hooks){.window_resized = _mlx_windows_resized_hook, .active = false};
	instance->mlx_even_list[EVENT_WINDOW_GENERIC] = (t_mlx_hooks){.window_generic = _mlx_windows_event_hook, .active = false};
}

void _mlx_window_update_buffer(t_mlx_instance *mlx, t_mlx_window *win, t_mlx_image *image)
{
	win->image = (Image){
	    .width = image->width,
	    .height = image->height,
	    .format = image->format,
	    .mipmaps = image->mimaps,
	    .data = image->buffer,
	};
	image->active_buffer = &win->active_image;

	win->active_image = true;
	win->texture = (Texture2D){
	    .id = image->texture.id,
	    .width = image->texture.width,
	    .height = image->texture.height,
	    .format = image->texture.format,
	    .mipmaps = image->texture.mipmaps,
	};
	image->active_texture = &win->active_texture;
	win->active_texture = true;
}
