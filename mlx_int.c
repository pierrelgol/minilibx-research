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

void _mlx_init_event_list(t_mlx_instance *const instance)
{
	t_mlx_hooks *hooks;

	hooks = instance->mlx_even_list;
	hooks[MLX_GENERIC_KEY_HOOK] = (t_mlx_hooks){.active = false};
	hooks[MLX_GENERIC_MOUSE_HOOK] = (t_mlx_hooks){.active = false};
	hooks[MLX_GENERIC_WINDOW_HOOK] = (t_mlx_hooks){.active = false};
	hooks[MLX_KEY_PRESSED] = (t_mlx_hooks){.active = false};
	hooks[MLX_KEY_RELEASED] = (t_mlx_hooks){.active = false};
	hooks[MLX_BUTTON_PRESSED] = (t_mlx_hooks){.active = false};
	hooks[MLX_BUTTON_RELEASED] = (t_mlx_hooks){.active = false};
	hooks[MLX_MOUVE_MOVE] = (t_mlx_hooks){.active = false};
	hooks[MLX_WINDOW_CLOSE] = (t_mlx_hooks){.active = false};
	hooks[MLX_WINDOW_FOCUS_IN] = (t_mlx_hooks){.active = false};
	hooks[MLX_WINDOW_FOCUS_OUT] = (t_mlx_hooks){.active = false};
	hooks[MLX_WINDOW_ENTER] = (t_mlx_hooks){.active = false};
	hooks[MLX_WINDOW_LEAVE] = (t_mlx_hooks){.active = false};
	hooks[MLX_WINDOW_RESIZE] = (t_mlx_hooks){.active = false};
	hooks[MLX_LOOP_HOOK] = (t_mlx_hooks){.active = false};
}


bool _mlx_should_broadcast_key_event(t_mlx_instance *mlx)
{
	t_mlx_hooks *hooks;

	hooks = mlx->mlx_even_list;
	return (hooks[MLX_KEY_PRESSED].active || hooks[MLX_KEY_RELEASED].active ||
	        hooks[MLX_GENERIC_KEY_HOOK].active);
}

bool _mlx_broadcast_keyboard_event(t_mlx_instance *mlx)
{
	t_mlx_hooks *key_pressed_hook;
	t_mlx_hooks *key_released_hook;
	t_mlx_hooks *key_generic_hook;
	int32_t      keycode;
	bool         autorepeat;


	keycode = GetKeyPressed();
	if (!keycode)
		return (false);
	autorepeat = mlx->autorepeat;
	key_pressed_hook = &mlx->mlx_even_list[MLX_KEY_PRESSED];
	key_released_hook = &mlx->mlx_even_list[MLX_KEY_RELEASED];
	key_generic_hook = &mlx->mlx_even_list[MLX_GENERIC_KEY_HOOK];
	if (key_generic_hook->active)
	{
		do
		{
			key_generic_hook->generic_key_hook(keycode, key_generic_hook->argument);
		} while (autorepeat && !IsKeyReleased(keycode));
	}
	else
	{
		do
		{
			key_pressed_hook->generic_key_hook(keycode, key_pressed_hook->argument);
		} while (autorepeat && !IsKeyReleased(keycode));
		if (key_released_hook->active)
			key_released_hook->key_released_hook(keycode, key_released_hook->argument);
	}
	return (true);
}
