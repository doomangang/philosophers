/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/25 17:49:06 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	pthread_t	super;
	int			i;

	gettimeofday(&share->start_time, 0);
	i = 0;
	while (i < share->arg->philo_num)
	{
		if (pthread_create(&share->tid[i], 0, routine, &share->p[i]))
			return (0);
		i++;
	}
	if (pthread_create(&super, 0, monitor, share))
		return (0);
	i = -1;
	while (++i < share->arg->philo_num)
		pthread_join(share->tid[i], 0);
	pthread_join(super, 0);
	return (1);
}

int	one_philo(t_share *share)
{
	pthread_t	super;

	gettimeofday(&share->start_time, 0);
	share->p[0].last_meal = share->start_time;
	if (pthread_create(&(share->tid[0]), 0, routine, &(share->p[0])))
		return (0);
	if (pthread_create(&super, 0, monitor, share))
		return (0);
	pthread_join(share->tid[0], 0);
	pthread_join(super, 0);
	return (1);
}

void	*routine(void *philo)
{
	t_philo		*p;
	t_share		*share;

	p = (t_philo *)philo;
	share = p->share;
	p->last_meal = share->start_time;
	print(THINK, p->num, share);
	if (p->num % 2)
		ft_usleep(20000);
	while (1)
	{
		take_a_fork(p, p->one_fork);
		take_a_fork(p, p->other_fork);
		eat(p);
		print(SLEEP, p->num, share);
		if (!all_alive(share))
			return (0);
		ft_usleep(share->arg->sleep_time * 1000);
		print(THINK, p->num, share);
		if (!all_alive(share))
			return (0);
		ft_usleep(200);
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
		pthread_mutex_lock(&(s->p[i].lock));
		if (timestamp(s->p[i].last_meal) >= s->arg->die_time && !s->p[i].eating)
		{
			pthread_mutex_unlock(&(s->p[i].lock));
			set_dead(s);
			print(DIE, i, share);
			return (0);
		}
		if (s->arg->must_eat != -1 && s->p[i].eat_count == s->arg->must_eat)
		{
			pthread_mutex_unlock(&(s->p[i].lock));
			set_dead(s);
			return (0);
		}
		pthread_mutex_unlock(&(s->p[i].lock));
		i = (i + 1) % s->arg->philo_num;
	}
	return (0);
}

void	eat(t_philo *p)
{
	if (!all_alive(p->share))
		return (drop_fork(p));
	print(EAT, p->num, p->share);
	pthread_mutex_lock(&(p->lock));
	p->eating = 1;
	gettimeofday(&p->last_meal, 0);
	pthread_mutex_unlock(&(p->lock));
	ft_usleep(p->share->arg->eat_time * 1000);
	drop_fork(p);
	pthread_mutex_lock(&(p->lock));
	p->eating = 0;
	p->eat_count += 1;
	pthread_mutex_unlock(&(p->lock));
}
