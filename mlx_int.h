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

#define MLX_ON_KEY_PRESS         02
#define MLX_ON_KEY_RELEASE       03
#define MLX_ON_BUTTON_PRESS      04
#define MLX_ON_BUTTON_RELEASE    05
#define MLX_ON_MOTION_NOTIFY     06
#define MLX_ON_ENTER_NOTIFY      07
#define MLX_ON_LEAVE_NOTIFY      08
#define MLX_ON_FOCUS_IN          09
#define MLX_ON_FOCUS_OUT         10
#define MLX_ON_KEYMAP_NOTIFY     11
#define MLX_ON_EXPOSE            12
#define MLX_ON_GRAPHICS_EXPOSE   13
#define MLX_ON_NO_EXPOSE         14
#define MLX_ON_VISIBILITY_NOTIFY 15
#define MLX_ON_CREATE_NOTIFY     16
#define MLX_ON_DESTROY_NOTIFY    17
#define MLX_ON_UNMAP_NOTIFY      18
#define MLX_ON_MAP_NOTIFY        19
#define MLX_ON_MAP_REQUEST       20
#define MLX_ON_REPARENT_NOTIFY   21
#define MLX_ON_CONFIGURE_NOTIFY  22
#define MLX_ON_CONFIGURE_REQUEST 23
#define MLX_ON_GRAVITY_NOTIFY    24
#define MLX_ON_RESIZE_REQUEST    25

#define MLX_NO_EVENT_MASK              (0L)
#define MLX_KEY_PRESS_MASK             (1L << 0)
#define MLX_KEY_RELEASE_MASK           (1L << 1)
#define MLX_BUTTON_PRESS_MASK          (1L << 2)
#define MLX_BUTTON_RELEASE_MASK        (1L << 3)
#define MLX_ENTER_WINDOW_MASK          (1L << 4)
#define MLX_LEAVE_WINDOW_MASK          (1L << 5)
#define MLX_POINTER_MOTION_MASK        (1L << 6)
#define MLX_POINTER_MOTION_HINT_MASK   (1L << 7)
#define MLX_BUTTON_1_MOTION_MASK       (1L << 8)
#define MLX_BUTTON_2_MOTION_MASK       (1L << 9)
#define MLX_BUTTON_3_MOTION_MASK       (1L << 10)
#define MLX_BUTTON_4_MOTION_MASK       (1L << 11)
#define MLX_BUTTON_5_MOTION_MASK       (1L << 12)
#define MLX_BUTTON_MOTION_MASK         (1L << 13)
#define MLX_KEYMAP_STATE_MASK          (1L << 14)
#define MLX_EXPOSURE_MASK              (1L << 15)
#define MLX_VISIBILITY_CHANGE_MASK     (1L << 16)
#define MLX_STRUCTURE_NOTIFY_MASK      (1L << 17)
#define MLX_RESIZE_REDIRECT_MASK       (1L << 18)
#define MLX_SUBSTRUCTURE_NOTIFY_MASK   (1L << 19)
#define MLX_SUBSTRUCTURE_REDIRECT_MASK (1L << 20)
#define MLX_FOCUS_CHANGE_MASK          (1L << 21)
#define MLX_PROPERTY_CHANGE_MASK       (1L << 22)
#define MLX_COLORMAP_CHANGE_MASK       (1L << 23)
#define MLX_OWNER_GRAB_BUTTON_MASK     (1L << 24)

#define MLX_DEFAULT_FONT_SIZE 10
#define MLX_MAX_EVENT 16
#define MLX_COLOR_FORMAT PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
#define IGNORE_ARGUMENT(x)                                                 \
	do                                                                     \
	{                                                                      \
		(void) (x);                                                        \
		printf("Argument '%s' ignored in function '%s'.\n", #x, __func__); \
	} while (0)

typedef int32_t (*t_no_op)(void);
typedef int32_t (*t_mouse_move_hook)(int32_t mouse_x, int32_t mouse_y, void *argument);
typedef int32_t (*t_mouse_pressed_hook)(int32_t bouton, void *argument);
typedef int32_t (*t_mouse_released_hook)(int32_t bouton, void *argument);
typedef int32_t (*t_key_pressed_hook)(int32_t keycode, void *argument);
typedef int32_t (*t_key_released_hook)(int32_t keycode, void *argument);
typedef int32_t (*t_loop_hook_begins)(void *argument);
typedef int32_t (*t_loop_hook_ends)(void *argument);
typedef int32_t (*t_windows_resized_hook)(int32_t width, int32_t height, void *argument);
typedef int32_t (*t_windows_event_hook)(int32_t event, void *argument);
typedef int32_t (*t_generic_mouse)();
typedef int32_t (*t_generic_keyboard)(int32_t keycode, void *argument);
typedef int32_t (*t_generic_window)();

typedef struct s_mlx_image    t_mlx_image;
typedef struct s_mlx_window   t_mlx_window;
typedef struct s_mlx_instance t_mlx_instance;

typedef enum e_mlx_event_kind
{
	EVENT_NO_EVENT = 0,
	EVENT_GENERIC_KEYBOARD = 1,
	EVENT_GENERIC_MOUSE = 2,
	EVENT_GENERIC_WINDOW = 3,
	EVENT_MOUSE_MOVE = 4,
	EVENT_MOUSE_PRESSED = 5,
	EVENT_MOUSE_RELEASED = 6,
	EVENT_KEY_PRESSED = 7,
	EVENT_KEY_RELEASED = 8,
	EVENT_LOOP_BEGINS = 9,
	EVENT_LOOP_ENDS = 10,
	EVENT_WINDOW_RESIZED = 11,
	EVENT_WINDOW_GENERIC = 12,
	EVENT_GENERIC = 13,

} t_mlx_event_kind;

typedef struct s_mlx_hooks
{
	t_mlx_event_kind kind;
	bool             active;
	union
	{
		t_no_op                no_op;
		t_generic_keyboard     generic_keyboard;
		t_generic_mouse        generic_mouse;
		t_generic_window       generic_window;
		t_mouse_move_hook      mouse_move;
		t_mouse_pressed_hook   mouse_pressed;
		t_mouse_released_hook  mouse_released;
		t_key_pressed_hook     key_pressed;
		t_key_released_hook    key_released;
		t_loop_hook_begins     loop_begins;
		t_windows_resized_hook window_resized;
		t_windows_event_hook   window_generic;
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
	bool *active_buffer;

	Texture2D texture;
	bool     *active_texture;
};

struct s_mlx_window
{
	t_mlx_instance *_parent;
	char           *title;
	int32_t         height;
	int32_t         width;

	Image image;
	bool  active_image;

	Texture2D texture;
	bool      active_texture;
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
};

void _mlx_init_event_list(t_mlx_instance *const instance);
void _mlx_window_update_buffer(t_mlx_instance *mlx, t_mlx_window *win, t_mlx_image *image);

bool _mlx_keyboard_event_has_subscribers(t_mlx_instance *mlx);
bool _mlx_mouse_event_has_subscribers(t_mlx_instance *mlx);
bool _mlx_window_event_has_subscribers(t_mlx_instance *mlx);
bool _mlx_generic_event_has_subscribers(t_mlx_instance *mlx);

void _mlx_try_broadcast_keyboard_event(t_mlx_instance *mlx, t_mlx_hooks *hook);
void _mlx_try_broadcast_mouse_event(t_mlx_instance *mlx, t_mlx_hooks *hook);
void _mlx_try_broadcast_window_event(t_mlx_instance *mlx, t_mlx_hooks *hook);
void _mlx_try_broadcast_generic_event(t_mlx_instance *mlx, t_mlx_hooks *hook);

#endif
