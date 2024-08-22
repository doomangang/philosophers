/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:14:46 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/22 22:02:46 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(struct timeval tv)
{
	struct timeval	now;
	time_t			sec;
	time_t			usec;

	gettimeofday(&now, 0);
	sec = now.tv_sec - tv.tv_sec;
	usec = now.tv_usec - tv.tv_usec;
	return (sec * 1000 + usec / 1000);
}

int	take_fork(t_philo *p)
{
	if (all_alive(p->share))
		pthread_mutex_lock(&(p->share->fork[p->one_fork]));
	print(FORK, p);
	if (all_alive(p->share) && p->share->arg->philo_num > 1)
		pthread_mutex_lock(&(p->share->fork[p->ano_fork]));
	else
	{
		pthread_mutex_unlock(&(p->share->fork[p->one_fork]));
		return (0);
	}
	print(FORK, p);
	return (1);
}

void	print(int status, t_philo *philo)
{
	int	i;

	i = philo->num;
	pthread_mutex_lock(&(philo->share->print));
	if (!all_alive(philo->share) && status != DIE)
	{
		pthread_mutex_unlock(&(philo->share->print));
		return ;
	}
	if (status == FORK)
		printf("%lld %d has taken a fork\n", get_time(philo->share->start), i);
	else if (status == EAT)
		printf("%lld %d is eating\n", get_time(philo->share->start), i);
	else if (status == SLEEP)
		printf("%lld %d is sleeping\n", get_time(philo->share->start), i);
	else if (status == THINK)
		printf("%lld %d is thinking\n", get_time(philo->share->start), i);
	else if (status == DIE)
		printf("%lld %d died\n", get_time(philo->share->start), i);
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
