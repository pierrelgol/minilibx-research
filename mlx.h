/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 12:41:43 by pollivie          #+#    #+#             */
/*   Updated: 2024/08/14 12:41:44 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_H
#define MLX_H

#include "mlx_int.h"
#define mlx_log(fmt, ...) \
	mlx_log_log(__FILE__, __FUNCTION__, fmt, ##__VA_ARGS__)

void mlx_log_log(const char *file, const char *function, const char *const fmt, ...);

void *mlx_instance_create(void);
void  mlx_instance_destroy(void *mlx_ptr);
void  mlx_instance_loop(void *mlx_ptr);
void *mlx_window_create(void *mlx_ptr, int32_t width, int32_t height, char *title);
void mlx_window_destroy(void *mlx_ptr, void *win_ptr);
void mlx_window_put_image(void *win_ptr, void *img_ptr, int32_t x, int32_t y);

void *mlx_image_create(void *mlx_ptr, int32_t width, int32_t height);
void  mlx_image_destroy(void *mlx_ptr, void *img_ptr);
char *mlx_image_buffer(void *img_ptr, int32_t *img_width, int32_t *img_height, int32_t *img_bpp);
void mlx_image_put_pixel(void *mlx_ptr, int32_t x, int32_t y, int32_t color);

void mlx_key_hooks(void *mlx_ptr, int32_t (*key_hook_fn)(int32_t keycode, void *argument), void *argument);

#endif
