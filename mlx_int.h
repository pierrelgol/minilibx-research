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
#include <stdio.h>
#include <stdlib.h>

#define IGNORE_ARGUMENT(x)                                                 \
	do                                                                     \
	{                                                                      \
		(void) (x);                                                        \
		printf("Argument '%s' ignored in function '%s'.\n", #x, __func__); \
	} while (0)

#define MLX_DEFAULT_FONT_SIZE 10
#define MLX_MAX_EVENT 16
#define MLX_COLOR_FORMAT PIXELFORMAT_UNCOMPRESSED_R8G8B8A8

typedef struct s_mlx_image    t_mlx_image;
typedef struct s_mlx_window   t_mlx_window;
typedef struct s_mlx_instance t_mlx_instance;

typedef int32_t (*t_mlx_generic_key_hook)(int32_t keycode, void *argument);
typedef int32_t (*t_mlx_generic_mouse_hook)(int32_t button, void *argument);
typedef int32_t (*t_mlx_generic_window_hook)(void *argument);

typedef int32_t (*t_mlx_key_pressed_hook)(int32_t keycode, void *argument);
typedef int32_t (*t_mlx_key_released_hook)(int32_t keycode, void *argument);
typedef int32_t (*t_mlx_mouse_button_pressed_hook)(int32_t button, void *argument);
typedef int32_t (*t_mlx_mouse_button_released_hook)(int32_t button, void *argument);
typedef int32_t (*t_mlx_mouse_get_motion_hook)(int32_t x, int32_t y, void *argument);
typedef int32_t (*t_mlx_window_close_hook)(void *argument);
typedef int32_t (*t_mlx_window_focus_in_hook)(void *argument);
typedef int32_t (*t_mlx_window_focus_out_hook)(void *argument);
typedef int32_t (*t_mlx_window_enter_notify_hook)(void *argument);
typedef int32_t (*t_mlx_window_leave_notify_hook)(void *argument);
typedef int32_t (*t_mlx_window_resize_notify_hook)(int32_t *new_x, int32_t *new_y, void *argument);
typedef int32_t (*t_mlx_loop_hook)(void *argument);
typedef int32_t (*t_mlx_generic_hook)();

typedef enum e_mlx_event_kind
{
	MLX_GENERIC_KEY_HOOK = 0,
	MLX_GENERIC_MOUSE_HOOK,
	MLX_GENERIC_WINDOW_HOOK,
	MLX_KEY_PRESSED,
	MLX_KEY_RELEASED,
	MLX_BUTTON_PRESSED,
	MLX_BUTTON_RELEASED,
	MLX_MOUVE_MOVE,
	MLX_WINDOW_CLOSE,
	MLX_WINDOW_FOCUS_IN,
	MLX_WINDOW_FOCUS_OUT,
	MLX_WINDOW_ENTER,
	MLX_WINDOW_LEAVE,
	MLX_WINDOW_RESIZE,
	MLX_LOOP_HOOK,

} t_mlx_event_kind;

typedef struct s_mlx_hooks
{
	t_mlx_event_kind kind;
	bool             active;
	union
	{
		t_mlx_generic_key_hook           generic_key_hook;
		t_mlx_generic_mouse_hook         generic_mouse_hook;
		t_mlx_generic_window_hook        generic_window_hook;
		t_mlx_key_pressed_hook           key_pressed_hook;
		t_mlx_key_released_hook          key_released_hook;
		t_mlx_mouse_button_pressed_hook  mouse_button_pressed_hook;
		t_mlx_mouse_button_released_hook mouse_button_released_hook;
		t_mlx_mouse_get_motion_hook      mouse_get_motion_hook;
		t_mlx_window_close_hook          window_close_hook;
		t_mlx_window_focus_in_hook       window_focus_in_hook;
		t_mlx_window_focus_out_hook      window_focus_out_hook;
		t_mlx_window_enter_notify_hook   window_enter_notify_hook;
		t_mlx_window_leave_notify_hook   window_leave_notify_hook;
		t_mlx_window_resize_notify_hook  window_resize_notify_hook;
		t_mlx_loop_hook                  loop_hook;
		t_mlx_generic_hook               generic_hook;
	};
	void *argument;
} t_mlx_hooks;

struct s_mlx_image
{
	int32_t height;
	int32_t width;
	int32_t format;
	int32_t mimaps;
	char *buffer;
	Texture2D texture;
	Image	image;
	bool	is_loaded;
};

struct s_mlx_window
{
	t_mlx_instance *_parent;
	char           *title;
	int32_t         height;
	int32_t         width;
	t_mlx_image    *_prev;
	Texture2D       texture;
	Image		image;
};

struct s_mlx_instance
{
	t_mlx_window *window;
	t_mlx_image  *_image;
	t_mlx_hooks   mlx_even_list[MLX_MAX_EVENT];
	int32_t       format;
	Font          font;
	int32_t       font_size;
	int32_t       autorepeat;
	int32_t       loop_ends;
	int32_t	      print_fps;
};

void _mlx_init_event_list(t_mlx_instance *const instance);

bool _mlx_should_broadcast_key_event(t_mlx_instance *mlx);
bool _mlx_broadcast_keyboard_event(t_mlx_instance *mlx);

#endif
