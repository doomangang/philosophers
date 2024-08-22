/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/22 21:33:15 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	int			i;
	pthread_t	super;

	gettimeofday(&share->start, 0);
	i = 0;
	while (i < share->arg->philo_num)
	{
		if (pthread_create(&share->tid[i], 0, routine, &share->p[i]))
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
	pthread_t	super;

	gettimeofday(&share->start, 0);
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

	p = (t_philo *)philo;
	pthread_mutex_lock(&p->lock);
	p->last.tv_sec = p->share->start.tv_sec;
	p->last.tv_usec = p->share->start.tv_usec;
	pthread_mutex_unlock(&p->lock);
	if (p->num % 2)
		ft_usleep(500);
	while (all_alive(p->share))
	{
		if (all_alive(p->share))
			pthread_mutex_lock(&(p->share->fork[p->one_fork]));
		print(FORK, p);
		if (all_alive(p->share))
			pthread_mutex_lock(&(p->share->fork[p->ano_fork]));
		else
			pthread_mutex_unlock(&(p->share->fork[p->one_fork]));
		print(FORK, p);
		eat(p);
		print(SLEEP, p);
		ft_usleep(p->share->arg->sleep_time * 1000);
		print(THINK, p);
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
		if (get_time(s->p[i].last) >= s->arg->die_time && !s->p[i].eating)
		{
			pthread_mutex_unlock(&(s->p[i].lock));
			set_dead(s);
			print(DIE, &s->p[i]);
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
	{
		pthread_mutex_unlock(&(p->share->fork[p->ano_fork]));
		pthread_mutex_unlock(&(p->share->fork[p->one_fork]));
		return ;
	}
	print(EAT, p);
	pthread_mutex_lock(&(p->lock));
	gettimeofday(&(p->last), 0);
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
