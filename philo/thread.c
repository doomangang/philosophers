/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/17 18:47:43 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	// pthread_t	*super;
	int			i;
	int			status;

	share->start_time = get_time();
	// if (pthread_create(super, 0, &monitor, &share))
	// 	return (0);
	i = 0;
	while (i < share->arg->philo_num)
	{
		share->philo[i].die_time = share->start_time + share->arg->die_time;
		if (pthread_create(&share->tid[i], 0, &routine, &share->philo[i]))
			return (0);
		i++;
	}
	i = 0;
	while (i < share->arg->philo_num)
	{
		pthread_join(share->tid[i], (void **)&status);
		i++;
	}
	// exit_process(share);
	return (status);
}

int	one_philo(t_share *share)
{
	share->start_time = get_time();
	share->philo[0].die_time = share->start_time + share->arg->die_time;
	if (pthread_create(&share->tid[0], 0, &routine, &share->philo))
		return (0);
	pthread_detach(share->tid[0]);
	// exit_process(share);
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
		if (!p->share->end_flag && get_time() < p->die_time - p->share->arg->eat_time)
		{
			pthread_mutex_lock(&(p->share->fork[p->l_fork]));
			print("fork", p);
			pthread_mutex_lock(&(p->share->fork[p->r_fork]));
			print("fork", p);
			print("eat", p);
			usleep(p->share->arg->eat_time);
			pthread_mutex_unlock(&(p->share->fork[p->l_fork]));
			pthread_mutex_unlock(&(p->share->fork[p->r_fork]));
			p->die_time = get_time() + p->share->arg->die_time;
		}
		if (!p->share->end_flag && get_time() < p->die_time - p->share->arg->sleep_time)
		{
			print("sleep", p);
			usleep(p->share->arg->sleep_time);
		}
		if (!p->share->end_flag && get_time() < p->die_time)
			print("think", p);
		if (!p->share->end_flag && get_time() >= p->die_time)
		{
			p->share->end_flag = 1;
			print("die", p);
			break ;
		}
	}
	return (0);
}

// void	*monitor(void *share)
// {

// }

// void	eat(t_philo *philo)
// {

// }
