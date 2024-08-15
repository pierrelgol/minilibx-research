/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_int.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 16:01:09 by pollivie          #+#    #+#             */
/*   Updated: 2024/08/14 16:01:09 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_INT_H
#define MLX_INT_H

#include "raygui.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <stdint.h>
#include <stdlib.h>

#define MLX_MAX_EVENT 16
#define MLX_COLOR_FORMAT PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
#define IGNORE_ARGUMENT(x) ((void) (x))

typedef int32_t (*t_mouse_move_hook)(int32_t mouse_x, int32_t mouse_y, void *argument);
typedef int32_t (*t_mouse_pressed_hook)(int32_t bouton, void *argument);
typedef int32_t (*t_mouse_released_hook)(int32_t bouton, void *argument);
typedef int32_t (*t_key_pressed_hook)(int32_t keycode, void *argument);
typedef int32_t (*t_key_released_hook)(int32_t keycode, void *argument);
typedef int32_t (*t_loop_hook_begins)(void *argument);
typedef int32_t (*t_loop_hook_ends)(void *argument);
typedef int32_t (*t_windows_resized_hook)(int32_t width, int32_t height, void *argument);
typedef int32_t (*t_windows_event_hook)(int32_t event, void *argument);

typedef struct s_mlx_image    t_mlx_image;
typedef struct s_mlx_window   t_mlx_window;
typedef struct s_mlx_instance t_mlx_instance;

typedef enum e_mlx_event_kind
{
	EVENT_NO_EVENT = 0,
	EVENT_MOUSE_MOVE = 1,
	EVENT_MOUSE_PRESSED = 2,
	EVENT_MOUSE_RELEASED = 3,
	EVENT_KEY_PRESSED = 4,
	EVENT_KEY_RELEASED = 5,
	EVENT_LOOP_BEGINS = 6,
	EVENT_LOOP_ENDS = 7,
	EVENT_WINDOW_RESIZED = 8,
	EVENT_WINDOW_GENERIC = 9,
	EVENT_GENERIC = 10,

} t_mlx_event_kind;

typedef struct s_mlx_hooks
{
	t_mlx_event_kind kind;
	int32_t          arg1;
	int32_t          arg2;
	int32_t          arg3;
	int32_t          arg4;
	void            *argument;
	union
	{
		t_mouse_move_hook      mouse_move;
		t_mouse_pressed_hook   mouse_pressed;
		t_mouse_released_hook  mouse_released;
		t_key_pressed_hook     key_pressed;
		t_key_released_hook    key_released;
		t_loop_hook_begins     loop_begins;
		t_loop_hook_ends       loop_ends;
		t_windows_resized_hook window_resized;
		t_windows_event_hook   window_generic;
	};
} t_mlx_hooks;

struct s_mlx_image
{
	int32_t   height;
	int32_t   width;
	int32_t   format;
	int32_t   mimaps;
	uint8_t  *buffer;
	Texture2D texture;
	bool      is_loaded;
};

struct s_mlx_window
{
	char        *title;
	int32_t      height;
	int32_t      width;
	t_mlx_image *image;
	Texture2D    texture;
	Texture2D    *_texture;
};

struct s_mlx_instance
{
	t_mlx_window *window;
	bool          active_window;

	t_mlx_image *image;

	int32_t color_format;
	bool    is_dirty;
	bool    active_image;

	uint8_t     event_count;
	t_mlx_hooks mlx_even_list[MLX_MAX_EVENT];
};

#endif
