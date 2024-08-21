/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:14:46 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/21 22:17:49 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (long long)1000) + (tv.tv_usec / 1000));
}

void	print(int status, t_philo *philo)
{
	int	time;

	time = get_time() - philo->share->start_time;
	pthread_mutex_lock(&(philo->share->print));
	if (status == FORK && all_alive(philo->share))
		printf("%d %d has taken a fork\n", time, philo->num);
	else if (status == EAT && all_alive(philo->share))
		printf("%d %d is eating\n", time, philo->num);
	else if (status == SLEEP && all_alive(philo->share))
		printf("%d %d is sleeping\n", time, philo->num);
	else if (status == THINK && all_alive(philo->share))
		printf("%d %d is thinking\n", time, philo->num);
	else if (status == DIE)
		printf("%d %d died\n", time, philo->num);
	pthread_mutex_unlock(&(philo->share->print));
}


int	all_alive(t_share *share)
{
	pthread_mutex_lock(&(share->lock));
	if (share->end_flag)
	{
		pthread_mutex_unlock(&(share->lock));
		return (0);
	}
	pthread_mutex_unlock(&(share->lock));
	return (1);
}

void	set_dead(t_share *share)
{
	pthread_mutex_lock(&share->lock);
	share->end_flag = 1;
	pthread_mutex_unlock(&share->lock);
}
