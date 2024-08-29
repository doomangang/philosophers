/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 14:14:46 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/27 21:59:18 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_a_fork(t_philo *p, int num)
{
	while (1)
	{
		if (!all_alive(p->share))
		{
			if (num == p->other_fork)
			{
				pthread_mutex_lock(&p->share->fork[p->one_fork]);
				p->share->fork_up[p->one_fork] = 0;
				pthread_mutex_unlock(&p->share->fork[p->one_fork]);
			}
			return ;
		}
		pthread_mutex_lock(&p->share->fork[num]);
		if (!p->share->fork_up[num])
		{
			p->share->fork_up[num] = 1;
			pthread_mutex_unlock(&p->share->fork[num]);
			print(FORK, p->num, p->share);
			return ;
		}
		pthread_mutex_unlock(&p->share->fork[num]);
		usleep(200);
	}
}

int	eat_check(t_share *share)
{
	int	i;
	int	eat_count;

	if (share->arg->must_eat == -1)
		return (0);
	i = 0;
	while (i < share->arg->philo_num)
	{
		pthread_mutex_lock(&(share->p[i].lock));
		eat_count = share->p[i].eat_count;
		pthread_mutex_unlock(&(share->p[i].lock));
		if (eat_count < share->arg->must_eat)
			return (0);
		i++;
	}
	return (1);
}

void	print(int status, int i, t_share *share)
{
	pthread_mutex_lock(&(share->print));
	if (!all_alive(share) && status != DIE)
	{
		pthread_mutex_unlock(&(share->print));
		return ;
	}
	if (status == FORK)
		printf("%ld %d has taken a fork\n", timestamp(share->start_time), i);
	else if (status == EAT)
		printf("%ld %d is eating\n", timestamp(share->start_time), i);
	else if (status == SLEEP)
		printf("%ld %d is sleeping\n", timestamp(share->start_time), i);
	else if (status == THINK)
		printf("%ld %d is thinking\n", timestamp(share->start_time), i);
	else if (status == DIE)
		printf("%ld %d died\n", timestamp(share->start_time), i);
	pthread_mutex_unlock(&(share->print));
}

int	all_alive(t_share *share)
{
	pthread_mutex_lock(&share->end_lock);
	if (share->end_flag)
	{
		pthread_mutex_unlock(&share->end_lock);
		return (0);
	}
	pthread_mutex_unlock(&share->end_lock);
	return (1);
}

void	set_dead(t_share *share)
{
	pthread_mutex_lock(&share->end_lock);
	share->end_flag = 1;
	pthread_mutex_unlock(&share->end_lock);
}
