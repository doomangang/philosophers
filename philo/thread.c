/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/24 14:51:10 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	int			i;

	gettimeofday(&share->start, 0);
	i = 0;
	while (i < share->arg->philo_num)
	{
		if (pthread_create(&share->tid[i], 0, routine, &share->p[i]))
			return (0);
		i++;
	}
	i = 0;
	while (i < share->arg->philo_num)
	{
		pthread_join(share->tid[i], 0);
		i++;
	}
	return (1);
}

int	one_philo(t_share *share)
{
	gettimeofday(&share->start, 0);
	if (pthread_create(&(share->tid[0]), 0, routine, &(share->p[0])))
		return (0);
	pthread_join(share->tid[0], 0);
	return (1);
}

void	*routine(void *philo)
{
	t_philo		*p;

	p = (t_philo *)philo;
	p->last.tv_sec = p->share->start.tv_sec;
	p->last.tv_usec = p->share->start.tv_usec;
	if (p->num % 2)
		print(THINK, p);
	while (all_alive(p->share))
	{
		if (take_fork(p))
		{
			eat(p);
			print(SLEEP, p);
			if (all_alive(p->share))
				ft_usleep(p->share->arg->sleep_time * 1000);
			print(THINK, p);
			if (all_alive(p->share))
				ft_usleep(200);
		}
	}
	return (0);
}

void	eat(t_philo *p)
{
	if (!all_alive(p->share))
	{
		pthread_mutex_unlock(&(p->share->fork[p->ano_fork]));
		pthread_mutex_unlock(&(p->share->fork[p->one_fork]));
		return ;
	}
	print(EAT, p);
	pthread_mutex_lock(&(p->lock));
	gettimeofday(&(p->last), 0);
	p->eating = 1;
	ft_usleep(p->share->arg->eat_time * 1000);
	pthread_mutex_unlock(&(p->lock));
	pthread_mutex_unlock(&(p->share->fork[p->ano_fork]));
	pthread_mutex_unlock(&(p->share->fork[p->one_fork]));
	pthread_mutex_lock(&(p->lock));
	p->eating = 0;
	p->eat_count += 1;
	pthread_mutex_unlock(&(p->lock));
}
