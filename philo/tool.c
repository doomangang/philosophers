/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:53:53 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/16 18:10:38 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	valid_input(char **av)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (av[i])
	{
		while (av[i][j])
		{
			if (av[i][j] == ' ')
			{
				j++;
				continue ;
			}
			if (!(av[i][j] >= 48 && av[i][j] <= 57))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int		sign;
	int		itgr;

	sign = 1;
	itgr = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= 48 && *str <= 57)
	{
		itgr = itgr * 10 + *str - '0';
		str++;
	}
	return (sign * itgr);
}

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (long long)1000) + (tv.tv_usec / 1000));
}

int	print_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	while (len--)
		write(2, str++, 1);
	return (0);
}
