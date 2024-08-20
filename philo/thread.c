/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/20 20:18:57 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	int			i;

	share->start_time = get_time();
	i = 0;
	while (i < share->arg->philo_num)
	{
		pthread_mutex_lock(&(share->philo[i].lock));
		share->philo[i].die_when = share->start_time + share->arg->die_time;
		pthread_mutex_unlock(&(share->philo[i].lock));
		if (pthread_create(&share->tid[i], 0, routine, &share->philo[i]))
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
	pthread_t	super;

	share->start_time = get_time();
	pthread_mutex_lock(&(share->philo[0].lock));
	share->philo[0].die_when = share->start_time + share->arg->die_time;
	pthread_mutex_unlock(&(share->philo[0].lock));
	if (pthread_create(&super, 0, monitor, &share))
		return (0);
	if (pthread_create(&share->tid[0], 0, routine, &share->philo[0]))
		return (0);
	pthread_detach(share->tid[0]);
	return (1);
}

void	*routine(void *philo)
{
	t_philo		*p;
	pthread_t	super;
	int			one_fork;
	int			other_fork;

	p = (t_philo *)philo;
	if (pthread_create(&super, 0, monitor, p))
		return (0);
	if (p->num % 2)
	{
		one_fork = p->l_fork;
		other_fork = p->r_fork;
		usleep(1000);
	}
	else
	{
		one_fork = p->r_fork;
		other_fork = p->l_fork;
	}
	while (1)
	{
		pthread_mutex_lock(&(p->share->lock));
		if (!p->share->end_flag)
		{
			pthread_mutex_unlock(&(p->share->lock));
			pthread_mutex_lock(&(p->share->fork[one_fork]));
			print("fork", p);
		}
		pthread_mutex_lock(&(p->share->lock));
		if (!p->share->end_flag)
		{
			pthread_mutex_unlock(&(p->share->lock));
			pthread_mutex_lock(&(p->share->fork[other_fork]));
			print("fork", p);
			eat(p);
		}
	}
	pthread_join(super, 0);
	return (0);
}

void	*monitor(void *philo)
{
	t_philo	*p;
	int		must_eat;

	p = (t_philo *)philo;
	must_eat = p->share->arg->must_eat;
	while (1)
	{
		pthread_mutex_lock(&(p->lock));
		if (p->die_when <= get_time() && p->eating == 0)
		{
			pthread_mutex_unlock(&(p->lock));
			p->share->end_flag = 1;
			print("die", p);
			return (0);
		}
		if (must_eat != -1 && p->eat_count == must_eat)
		{
			p->share->end_flag = 1;
			pthread_mutex_unlock(&(p->lock));
			return (0);
		}
		pthread_mutex_unlock(&(p->lock));
		usleep(10);
	}
	return (0);
}

void	eat(t_philo *p)
{
	pthread_mutex_lock(&(p->lock));
	p->die_when = get_time() + p->share->arg->die_time;
	pthread_mutex_unlock(&(p->lock));
	print("eat", p);
	p->eating = 1;
	usleep(p->share->arg->eat_time * 1000);
	p->eating = 0;
	pthread_mutex_unlock(&(p->share->fork[p->l_fork]));
	pthread_mutex_unlock(&(p->share->fork[p->r_fork]));
	pthread_mutex_lock(&(p->lock));
	p->eat_count += 1;
	pthread_mutex_unlock(&(p->lock));
	if (!p->share->end_flag)
	{
		print("sleep", p);
		usleep(p->share->arg->sleep_time * 1000);
	}
	if (!p->share->end_flag)
		print("think", p);
}
