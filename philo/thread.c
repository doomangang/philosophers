/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/18 04:09:03 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	pthread_t	super;
	int			i;

	share->start_time = get_time();
	if (pthread_create(&super, 0, monitor, share))
		return (exit_process(share));
	i = 0;
	while (i < share->arg->philo_num)
	{
		pthread_mutex_lock(&(share->philo[i].lock));
		share->philo[i].die_when = share->start_time + share->arg->die_time;
		pthread_mutex_unlock(&(share->philo[i].lock));
		if (pthread_create(&share->tid[i], 0, routine, &share->philo[i]))
			return (exit_process(share));
		i++;
	}
	i = 0;
	while (i < share->arg->philo_num)
	{
		pthread_join(share->tid[i], 0);
		i++;
	}
	pthread_join(super, 0);
	exit_process(share);
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
		return (exit_process(share));
	if (pthread_create(&share->tid[0], 0, routine, &share->philo[0]))
		return (exit_process(share));
	pthread_detach(share->tid[0]);
	exit_process(share);
	return (1);
}

void	*routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	if (p->num % 2)
		usleep(1000);
	while (!p->share->end_flag)
	{
		pthread_mutex_lock(&p->lock);
		eat(p);
		pthread_mutex_unlock(&p->lock);
		print("sleep", p);
		usleep(p->share->arg->sleep_time);
		print("think", p);
	}
	return (0);
}

void	*monitor(void *share)
{
	t_share	*s;
	int		i;

	s = share;
	i = 0;
	while (!s->end_flag)
	{
		pthread_mutex_lock(&(s->philo[i].lock));
		if (s->philo[i].die_when <= get_time())
		{
			pthread_mutex_lock(&(s->lock));
			s->end_flag = 1;
			pthread_mutex_unlock(&(s->lock));
			print("die", &s->philo[i]);
		}
		else if (s->arg->must_eat != -1 && s->philo[i].eat_count == s->arg->must_eat)
		{
			pthread_mutex_lock(&(s->lock));
			s->end_flag = 1;
			pthread_mutex_unlock(&(s->lock));
		}
		pthread_mutex_unlock(&(s->philo[i].lock));
		i = (i + 1) % s->arg->philo_num;
	}
	return (0);
} // philo mutex?

void	eat(t_philo *p)
{
	if (p->num % 2)
	{
		if (!pthread_mutex_lock(&(p->share->fork[p->l_fork])))
			print("fork", p);
		if (!pthread_mutex_lock(&(p->share->fork[p->r_fork])))
			print("fork", p);
	}
	else
	{
		if (!pthread_mutex_lock(&(p->share->fork[p->r_fork])))
			print("fork", p);
		if (!pthread_mutex_lock(&(p->share->fork[p->l_fork])))
			print("fork", p);
	}
	print("eat", p);
	usleep(p->share->arg->eat_time);
	pthread_mutex_unlock(&(p->share->fork[p->l_fork]));
	pthread_mutex_unlock(&(p->share->fork[p->r_fork]));
	pthread_mutex_lock(&p->lock);
	p->die_when = get_time() + p->share->arg->die_time;
	p->eat_count += 1;
	pthread_mutex_unlock(&p->lock);
}
