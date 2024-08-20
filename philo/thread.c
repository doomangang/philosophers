/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/20 19:21:26 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	pthread_t	super;
	int			i;

	share->start_time = get_time();
	if (pthread_create(&super, 0, monitor, share))
		return (0);
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
	pthread_join(super, 0);
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
	t_philo	*p;
	int		one_fork;
	int		other_fork;

	p = (t_philo *)philo;
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
		if (p->share->end_flag)
			break ;
		pthread_mutex_lock(&(p->share->fork[one_fork]));
		print("fork", p);
		if (p->share->end_flag)
		{
			pthread_mutex_unlock(&(p->share->fork[one_fork]));
			break ;
		}
		pthread_mutex_lock(&(p->share->fork[other_fork]));
		print("fork", p);
		eat(p);
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
		if (s->philo[i].die_when <= get_time() && s->philo[i].eating == 0)
		{
			s->end_flag = 1;
			print("die", &s->philo[i]);
			return (0);
		}
		if (s->arg->must_eat != -1 && s->philo[i].eat_count == s->arg->must_eat)
		{
			s->end_flag = 1;
			return (0);
		}
		i = (i + 1) % s->arg->philo_num;
		usleep(100);
	}
	return (0);
}

void	eat(t_philo *p)
{
	print("eat", p);
	p->eating = 1;
	usleep(p->share->arg->eat_time * 1000);
	p->eating = 0;
	pthread_mutex_unlock(&(p->share->fork[p->l_fork]));
	p->share->fork_flag[p->l_fork] = 0;
	pthread_mutex_unlock(&(p->share->fork[p->r_fork]));
	p->share->fork_flag[p->r_fork] = 0;
	p->die_when = get_time() + p->share->arg->die_time;
	p->eat_count += 1;
	if (!p->share->end_flag)
	{
		print("sleep", p);
		usleep(p->share->arg->sleep_time * 1000);
	}
	if (!p->share->end_flag)
		print("think", p);
}
