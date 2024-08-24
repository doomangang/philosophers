/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:14:46 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/24 20:00:39 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	timestamp(struct timeval tv)
{
	struct timeval	now;
	time_t			sec;
	time_t			usec;

	gettimeofday(&now, 0);
	sec = now.tv_sec - tv.tv_sec;
	usec = now.tv_usec - tv.tv_usec;
	return (sec * 1000 + usec / 1000);
}

void	take_a_fork(t_philo *p, int num)
{
	pthread_mutex_lock(&p->share->fork[num]);
	while (p->share->f_stat[num])
	{
		pthread_mutex_unlock(&p->share->fork[num]);
		if (!is_alive(p, *(p->share->arg)))
		{
			if (num == p->right)
			{
				pthread_mutex_lock(&p->share->fork[p->left]);
				p->share->f_stat[p->left] = 0;
				pthread_mutex_unlock(&p->share->fork[p->left]);
			}
			return ;
		}
		pthread_mutex_lock(&p->share->fork[num]);
	}
	p->share->f_stat[num] = 1;
	pthread_mutex_unlock(&p->share->fork[num]);
	print(FORK, p, timestamp(p->share->start));
}

void	print(int status, t_philo *philo, long long time)
{
	int	i;
	t_share	share;

	i = philo->num;
	share = *(philo->share);
	pthread_mutex_lock(&(share.print));
	if (!is_alive(philo, *(share.arg)) && status != DIE)
	{
		pthread_mutex_unlock(&(share.print));
		return ;
	}
	if (status == FORK)
		printf("%lld %d has taken a fork\n", time, i);
	else if (status == EAT)
		printf("%lld %d is eating\n", time, i);
	else if (status == SLEEP)
		printf("%lld %d is sleeping\n", time, i);
	else if (status == THINK)
		printf("%lld %d is thinking\n", time, i);
	else if (status == DIE)
		printf("%lld %d died\n", time, i);
	pthread_mutex_unlock(&(share.print));
}

int	is_alive(t_philo *p, t_arg arg)
{
	pthread_mutex_lock(&p->share->lock);
	if (p->share->end_flag)
	{
		pthread_mutex_unlock(&p->share->lock);
		return (0);
	}
	pthread_mutex_unlock(&p->share->lock);
	pthread_mutex_lock(&(p->lock));
	if (timestamp(p->last) > arg.die_time && !p->eating)
	{
		pthread_mutex_unlock(&(p->lock));
		set_dead(p->share);
		print(DIE, p, timestamp(p->share->start));
		return (0);
	}
	if (arg.must_eat != -1 && p->eat_count == arg.must_eat)
	{
		pthread_mutex_unlock(&(p->lock));
		set_dead(p->share);
		return (0);
	}
	pthread_mutex_unlock(&(p->lock));
	return (1);
}

void	set_dead(t_share *share)
{
	pthread_mutex_lock(&share->lock);
	share->end_flag = 1;
	pthread_mutex_unlock(&share->lock);
}
