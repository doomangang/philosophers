/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/20 16:42:36 by jihyjeon         ###   ########.fr       */
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
		usleep(10);
	while (1)
	{
		pthread_mutex_lock(&(p->share->lock));
		if (p->share->end_flag)
		{
			pthread_mutex_unlock(&(p->share->lock));
				break ;
		}
		pthread_mutex_unlock(&(p->share->lock));
		if (p->share->fork_flag[p->l_fork])
			continue ;
		pthread_mutex_lock(&(p->share->fork[p->l_fork]));
		pthread_mutex_lock(&(p->share->lock));
		p->share->fork_flag[p->l_fork] = 1;
		pthread_mutex_unlock(&(p->share->lock));
		print("fork", p);
		if (!p->share->fork_flag[p->r_fork] && !p->share->end_flag)
		{
			pthread_mutex_lock(&(p->share->fork[p->r_fork]));
			pthread_mutex_lock(&(p->share->lock));
			p->share->fork_flag[p->r_fork] = 1;
			pthread_mutex_unlock(&(p->share->lock));
			print("fork", p);
			eat(p);
		}
		else
			pthread_mutex_unlock(&(p->share->fork[p->l_fork]));
	}
	return (0);
}

void	*monitor(void *share)
{
	t_share	*s;
	int		i;

	s = share;
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&(s->philo[i].lock));
		if (s->philo[i].die_when <= get_time() && s->philo[i].eating == 0)
		{
			pthread_mutex_lock(&(s->lock));
			s->end_flag = 1;
			pthread_mutex_unlock(&(s->lock));
			print("die", &s->philo[i]);
			pthread_mutex_unlock(&(s->philo[i].lock));
			return (0);
		}
			pthread_mutex_unlock(&(s->philo[i].lock));
		pthread_mutex_lock(&(s->lock));
		if (s->arg->must_eat != -1 && s->philo[i].eat_count == s->arg->must_eat)
		{
			s->end_flag = 1;
			pthread_mutex_unlock(&(s->lock));
			return (0);
		}
		pthread_mutex_unlock(&(s->lock));
		i = (i + 1) % s->arg->philo_num;
	}
	return (0);
}

void	eat(t_philo *p)
{
	print("eat", p);
	pthread_mutex_lock(&p->lock);
	p->eating = 1;
	pthread_mutex_unlock(&p->lock);
	usleep(p->share->arg->eat_time);
	pthread_mutex_lock(&p->lock);
	p->eating = 0;
	pthread_mutex_unlock(&(p->share->fork[p->l_fork]));
	pthread_mutex_unlock(&(p->share->fork[p->r_fork]));
	pthread_mutex_lock(&p->lock);
	p->die_when = get_time() + p->share->arg->die_time;
	p->eat_count += 1;
	pthread_mutex_unlock(&p->lock);
	if (!p->share->end_flag)
	{
		print("sleep", p);
		usleep(p->share->arg->sleep_time);
	}
	if (!p->share->end_flag)
		print("think", p);
}
