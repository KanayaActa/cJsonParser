/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtod_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 13:04:01 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/25 13:19:33 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	skip_spaces(t_strtod_ctx *c)
{
	while (ft_isspace((unsigned char)*(c->str)))
		c->str++;
}

void	parse_sign(t_strtod_ctx *c)
{
	c->sign = 1;
	if (*(c->str) == '+')
		c->str++;
	else if (*(c->str) == '-')
	{
		c->sign = -1;
		c->str++;
	}
}

bool	parse_infinity(t_strtod_ctx *c)
{
	if (ft_strncasecmp(c->str, "inf", 3) != 0)
		return (false);
	c->str += 3;
	if (ft_strncasecmp(c->str, "inity", 5) == 0)
		c->str += 5;
	if (c->endptr)
		*(c->endptr) = (char *)c->str;
	return (true);
}

void	parse_number_part(t_strtod_ctx *c)
{
	double	frac;

	while (ft_isdigit((unsigned char)*(c->str)))
	{
		c->has_digits = true;
		c->val = c->val * 10.0 + (*(c->str) - '0');
		c->str++;
	}
	if (*(c->str) == '.')
	{
		frac = 0.1;
		c->str++;
		while (ft_isdigit((unsigned char)*(c->str)))
		{
			c->has_digits = true;
			c->val += (*(c->str) - '0') * frac;
			frac *= 0.1;
			c->str++;
		}
	}
}

void	parse_exponent_part(t_strtod_ctx *c)
{
	int		exp_sign;
	bool	has_exp_digits;
	int		tmp_exp;

	if (*(c->str) != 'e' && *(c->str) != 'E')
		return ;
	c->str++;
	exp_sign = 1;
	if (*(c->str) == '+')
		c->str++;
	else if (*(c->str) == '-')
	{
		exp_sign = -1;
		c->str++;
	}
	has_exp_digits = false;
	tmp_exp = 0;
	while (ft_isdigit((unsigned char)*(c->str)))
	{
		has_exp_digits = true;
		tmp_exp = tmp_exp * 10 + (*(c->str) - '0');
		c->str++;
	}
	if (has_exp_digits)
		c->exponent += exp_sign * tmp_exp;
}
