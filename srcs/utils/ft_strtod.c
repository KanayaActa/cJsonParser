/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtod.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miwasa <miwasa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 11:32:44 by miwasa            #+#    #+#             */
/*   Updated: 2025/02/26 10:29:51 by miwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>

static double	finalize_no_digits(t_strtod_ctx *c)
{
	if (c->endptr)
		*(c->endptr) = (char *)c->start;
	return (0.0);
}

static double	finalize_infinity(t_strtod_ctx *c)
{
	double	ret;

	if (c->sign > 0)
		ret = HUGE_VAL;
	else
		ret = -HUGE_VAL;
	if (c->endptr)
		*(c->endptr) = (char *)c->str;
	return (ret);
}

static double	finalize_value(t_strtod_ctx *c)
{
	double	p;
	double	val;

	if (!c->has_digits)
		return (finalize_no_digits(c));
	p = pow(10.0, (double)c->exponent);
	val = c->val * p;
	if (c->sign < 0)
		val = -val;
	if (c->endptr)
		*(c->endptr) = (char *)c->str;
	if (!isfinite(val))
		errno = ERANGE;
	else if (val != 0.0 && fabs(val) < 1e-308)
		errno = ERANGE;
	return (val);
}

double	ft_strtod(const char *nptr, char **endptr)
{
	t_strtod_ctx	c;

	errno = 0;
	c.start = nptr;
	c.str = nptr;
	c.endptr = endptr;
	c.val = 0.0;
	c.sign = 1;
	c.exponent = 0;
	c.has_digits = false;
	skip_spaces(&c);
	parse_sign(&c);
	if (parse_infinity(&c))
		return (finalize_infinity(&c));
	parse_number_part(&c);
	parse_exponent_part(&c);
	return (finalize_value(&c));
}
