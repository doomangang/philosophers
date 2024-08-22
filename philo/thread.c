/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/22 16:59:10 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	int			i;
	pthread_t	super;

	share->start_time = get_time();
	i = 0;
	while (i < share->arg->philo_num)
	{
		if (pthread_create(&share->tid[i], 0, routine, &share->philo[i]))
			return (0);
		i++;
	}
	if (pthread_create(&super, 0, monitor, share))
		return (0);
	i = 0;
	while (i < share->arg->philo_num)
	{
		pthread_join(share->tid[i], 0);
		i++;
	}
	pthread_join(super, 0);
	return (1);
}

int	one_philo(t_share *share)
{
	share->start_time = get_time();
	if (pthread_create(&(share->tid[0]), 0, routine, &(share->philo[0])))
		return (0);
	pthread_detach(share->tid[0]);
	return (1);
}

void	*routine(void *philo)
{
	t_philo		*p;

	p = (t_philo *)philo;
	p->die_when = p->share->start_time + p->life_span;
	if (p->num % 2)
		ft_usleep(1000);
	while (all_alive(p->share))
	{
		pthread_mutex_lock(&(p->share->fork[p->one_fork]));
		print(FORK, p);
		pthread_mutex_lock(&(p->share->fork[p->ano_fork]));
		print(FORK, p);
		eat(p);
		print(SLEEP, p);
		ft_usleep(p->share->arg->sleep_time * 1000);
		print(THINK, p);
		ft_usleep(300);
	}
	return (0);
}

void	*monitor(void *share)
{
	t_share	*s;
	int		i;

	s = (t_share *)share;
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&(s->philo[i].lock));
		if (s->philo[i].die_when <= get_time() && s->philo[i].eating == 0)
		{
			pthread_mutex_unlock(&(s->philo[i].lock));
			set_dead(s);
			print(DIE, &(s->philo[i]));
			return (0);
		}
		if (s->arg->must_eat != -1 && s->philo[i].eat_count == s->arg->must_eat)
		{
			pthread_mutex_unlock(&(s->philo[i].lock));
			set_dead(s);
			return (0);
		}
		pthread_mutex_unlock(&(s->philo[i].lock));
		i = (i + 1) % s->arg->philo_num;
	}
	return (0);
}

void	eat(t_philo *p)
{
	print(EAT, p);
	pthread_mutex_lock(&(p->lock));
	p->die_when = get_time() + p->life_span;
	p->eating = 1;
	pthread_mutex_unlock(&(p->lock));
	ft_usleep(p->share->arg->eat_time * 1000);
	pthread_mutex_unlock(&(p->share->fork[p->ano_fork]));
	pthread_mutex_unlock(&(p->share->fork[p->one_fork]));
	pthread_mutex_lock(&(p->lock));
	p->eating = 0;
	p->eat_count += 1;
	pthread_mutex_unlock(&(p->lock));
}
