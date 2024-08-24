/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:53:53 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/24 19:49:26 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	heap_init(t_share *share, int num)
{
	share->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num);
	if (!share->fork)
		return (0);
	share->p = (t_philo *)malloc(sizeof(t_philo) * num);
	if (!share->p)
		return (0);
	share->tid = (pthread_t *)malloc(num * sizeof(pthread_t));
	if (!share->tid)
		return (0);
	return (1);
}

void	ft_usleep(long long sleep)
{
	struct timeval	tv;
	long long		start;

	if (gettimeofday(&tv, NULL))
		return ;
	start = tv.tv_sec * 1000000 + tv.tv_usec;
	while (tv.tv_sec * 1000000 + tv.tv_usec <= start + sleep)
	{
		usleep(50);
		if (gettimeofday(&tv, NULL))
			return ;
	}
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
