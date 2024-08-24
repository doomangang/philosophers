/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:14:46 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/24 22:41:23 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_a_fork(t_philo *p, int num)
{
	while (1)
	{
		if (!all_alive(p->share))
		{
			if (num == p->other)
			{
				pthread_mutex_lock(&p->share->fork[p->one]);
				p->share->f_stat[p->one] = 0;
				pthread_mutex_unlock(&p->share->fork[p->one]);
			}
			return ;
		}
		pthread_mutex_lock(&p->share->fork[num]);
		if (!p->share->f_stat[num])
		{
			p->share->f_stat[num] = 1;
			pthread_mutex_unlock(&p->share->fork[num]);
			print(FORK, p);
			return ;
		}
		pthread_mutex_unlock(&p->share->fork[num]);
		usleep(200);
	}
}

void	drop_fork(t_philo *p)
{
	pthread_mutex_lock(&(p->share->fork[p->other]));
	p->share->f_stat[p->other] = 0;
	pthread_mutex_unlock(&(p->share->fork[p->other]));
	pthread_mutex_lock(&(p->share->fork[p->one]));
	p->share->f_stat[p->one] = 0;
	pthread_mutex_unlock(&(p->share->fork[p->one]));
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
		printf("%ld %d has taken a fork\n", timestamp(philo->share->start), i);
	else if (status == EAT)
		printf("%ld %d is eating\n", timestamp(philo->share->start), i);
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", timestamp(philo->share->start), i);
	else if (status == THINK)
		printf("%ld %d is thinking\n", timestamp(philo->share->start), i);
	else if (status == DIE)
		printf("%ld %d died\n", timestamp(philo->share->start), i);
	pthread_mutex_unlock(&(philo->share->print));
}

int	all_alive(t_share *share)
{
	pthread_mutex_lock(&share->lock);
	if (share->end_flag)
	{
		pthread_mutex_unlock(&share->lock);
		return (0);
	}
	pthread_mutex_unlock(&share->lock);
	return (1);
}

void	set_dead(t_share *share)
{
	pthread_mutex_lock(&share->lock);
	share->end_flag = 1;
	pthread_mutex_unlock(&share->lock);
}
