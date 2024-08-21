/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/21 19:28:28 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	int	i;

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
	share->start_time = get_time();
	pthread_mutex_lock(&(share->philo[0].lock));
	share->philo[0].die_when = share->start_time + share->arg->die_time;
	pthread_mutex_unlock(&(share->philo[0].lock));
	if (pthread_create(&share->tid[0], 0, routine, &share->philo[0]))
		return (0);
	pthread_detach(share->tid[0]);
	return (1);
}

void	*routine(void *philo)
{
	t_philo		*p;
	pthread_t	super;

	p = (t_philo *)philo;
	if (pthread_create(&super, 0, monitor, p))
		return (0);
	if (p->num % 2)
		ft_usleep(1);
	while (all_alive(p->share))
	{
		if (all_alive(p->share))
			take_fork(p);
		if (all_alive(p->share))
			eat(p);
		if (all_alive(p->share))
		{
			print(SLEEP, p);
			ft_usleep(p->share->arg->sleep_time);
		}
		if (all_alive(p->share))
			print(THINK, p);
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
	while (all_alive(p->share))
	{
		pthread_mutex_lock(&(p->lock));
		if (p->die_when <= get_time() && p->eating == 0)
		{
			set_dead(p->share);
			print(DIE, p);
		}
		if (must_eat != -1 && p->eat_count == must_eat)
			set_dead(p->share);
		pthread_mutex_unlock(&(p->lock));
	}
	return (0);
}

void	eat(t_philo *p)
{
	pthread_mutex_lock(&(p->lock));
	p->die_when = get_time() + p->share->arg->die_time;
	print(EAT, p);
	p->eating = 1;
	ft_usleep(p->share->arg->eat_time);
	pthread_mutex_unlock(&(p->share->fork[p->one_fork]));
	pthread_mutex_unlock(&(p->share->fork[p->ano_fork]));
	p->eating = 0;
	p->eat_count += 1;
	pthread_mutex_unlock(&(p->lock));
}
