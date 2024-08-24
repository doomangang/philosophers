/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:53:53 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/24 22:44:21 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	valid_input(char **av)
{
	int	i;
	int	j;

	i = 0;
	while (av[i])
	{
		j = 0;
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

int	exit_process(t_share *share)
{
	int	i;

	i = 0;
	while (i < share->arg->p_cnt)
	{
		pthread_mutex_destroy(&share->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&share->print);
	free(share->p);
	free(share->tid);
	free(share->fork);
	return (0);
}

void	ft_usleep(long sleep)
{
	struct timeval	tv;
	long			start;

	if (gettimeofday(&tv, NULL))
		return ;
	start = tv.tv_sec * 1000000 + tv.tv_usec;
	while (1)
	{
		if (gettimeofday(&tv, NULL))
			return ;
		if (tv.tv_sec * 1000000 + tv.tv_usec > start + sleep)
			return ;
		usleep(100);
	}
}

long	timestamp(struct timeval tv)
{
	struct timeval	t;

	gettimeofday(&t, 0);
	return ((t.tv_sec - tv.tv_sec) * 1000 + (t.tv_usec - tv.tv_usec) / 1000);
}
