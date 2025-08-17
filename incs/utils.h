/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 07:12:09 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 11:09:30 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <float.h>
# include <math.h>

// 文字列操作関数
size_t		ft_strlen(const char *s);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memset(void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
char		*ft_strdup(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strncasecmp(const char *s1, const char *s2, size_t n);

// 文字判定関数
int			ft_isspace(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isupper(int c);
int			ft_islower(int c);
int			ft_tolower(int c);

// メモリ管理関数
void		*ft_calloc(size_t count, size_t size);
void		*xrealloc(void *ptr, size_t size);

// 出力関数
void		ft_putstr_fd(char *s, int fd);
void		print_error(const char *msg);

// 数値変換関数
typedef struct s_strtod_ctx
{
	const char	*str;
	const char	*start;
	char		**endptr;
	double		val;
	int			sign;
	int			exponent;
	bool		has_digits;
}				t_strtod_ctx;

double		ft_strtod(const char *nptr, char **endptr);
void		skip_spaces(t_strtod_ctx *c);
void		parse_sign(t_strtod_ctx *c);
bool		parse_infinity(t_strtod_ctx *c);
void		parse_number_part(t_strtod_ctx *c);
void		parse_exponent_part(t_strtod_ctx *c);

#endif
