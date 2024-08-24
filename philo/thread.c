/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/24 19:41:06 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	int			i;

	gettimeofday(&share->start, 0);
	i = -1;
	while (++i < share->arg->p_cnt)
		share->p[i].last = share->start;
	i = 0;
	while (i < share->arg->p_cnt)
	{
		if (pthread_create(&share->tid[i], 0, routine, &share->p[i]))
			return (0);
		i++;
	}
	i = -1;
	while (++i < share->arg->p_cnt)
		pthread_join(share->tid[i], 0);
	return (1);
}

int	one_philo(t_share *share)
{
	gettimeofday(&share->start, 0);
	share->p[0].last = share->start;
	if (pthread_create(&(share->tid[0]), 0, routine, &(share->p[0])))
		return (0);
	pthread_join(share->tid[0], 0);
	return (1);
}

void	*routine(void *philo)
{
	t_philo		*p;
	t_arg		arg;

	p = (t_philo *)philo;
	arg = *(p->share->arg);
	if (p->num % 2)
		print(THINK, p);
	while (1)
	{
		take_a_fork(p, p->left);
		take_a_fork(p, p->right);
		if (!is_alive(p, arg))
			return (0);
		eat(p);
		print(SLEEP, p);
		ft_usleep(is_alive(p, arg), p->share->arg->sleep_time * 1000);
		print(THINK, p);
		ft_usleep(is_alive(p, arg), 200);
		if (!is_alive(p, arg))
			return (0);
	}
	return (0);
}

void	eat(t_philo *p)
{
	if (!is_alive(p, *(p->share->arg)))
		return (drop_fork(p));
	print(EAT, p);
	pthread_mutex_lock(&(p->lock));
	gettimeofday(&p->last, 0);
	p->eating = 1;
	pthread_mutex_unlock(&(p->lock));
	ft_usleep(is_alive(p, *(p->share->arg)), p->share->arg->eat_time * 1000);
	drop_fork(p);
	pthread_mutex_lock(&(p->lock));
	p->eating = 0;
	p->eat_count += 1;
	pthread_mutex_unlock(&(p->lock));
}

void	drop_fork(t_philo *p)
{
	pthread_mutex_lock(&(p->share->fork[p->right]));
	p->share->f_stat[p->right] = 0;
	pthread_mutex_unlock(&(p->share->fork[p->right]));
	pthread_mutex_lock(&(p->share->fork[p->left]));
	p->share->f_stat[p->left] = 0;
	pthread_mutex_unlock(&(p->share->fork[p->left]));
}
