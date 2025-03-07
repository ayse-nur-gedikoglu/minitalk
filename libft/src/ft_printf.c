/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agedikog <agedikog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 13:29:35 by agedikog          #+#    #+#             */
/*   Updated: 2025/02/09 13:29:35 by agedikog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_change(char c, va_list arg)
{
	if (c == 'c')
		return (ft_putchar(va_arg(arg, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(arg, char *)));
	else if (c == 'd' || c == 'i')
		return (ft_putnbr(va_arg(arg, int)));
	else if (c == 'u')
		return (ft_putunbr(va_arg(arg, unsigned int)));
	else if (c == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(const char *s, ...)
{
	int		i;
	int		len;
	va_list	arg;

	i = 0;
	len = 0;
	va_start(arg, s);
	while (s[i])
	{
		if (s[i] == '%' && s[i + 1] != '\0')
		{
			len += ft_change(s[i + 1], arg);
			i++;
		}
		else
			len += ft_putchar(s[i]);
		i++;
	}
	va_end(arg);
	return (len);
}
