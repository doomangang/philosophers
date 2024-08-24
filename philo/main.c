/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:39:48 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/24 13:57:16 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_arg	arg;
	t_share	share;

	if (ac < 5 || ac > 6)
		return (0);
	if (!valid_input(&av[1]))
		return (0);
	if (!set_arg(ac, av, &arg))
		return (0);
	if (!heap_init(&share, arg.philo_num))
		return (0);
	if (!mutex_init(&share, &arg))
		return (0);
	if (!philo_init(&share))
		return (0);
	if (arg.philo_num == 1)
		return (one_philo(&share));
	if (!philo(&share))
		return (0);
	exit_process(&share);
	return (0);
}

int	valid_input(char **av)
{
	int	i;
	int	j;

	i = 0;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] == ' ')
			{
				j++;
				continue ;
			}
			if (!(av[i][j] >= 48 && av[i][j] <= 57))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	set_arg(int ac, char **av, t_arg *arg)
{
	memset(arg, 0, sizeof(t_arg));
	arg->philo_num = ft_atoi(av[1]);
	arg->die_time = ft_atoi(av[2]);
	arg->eat_time = ft_atoi(av[3]);
	arg->sleep_time = ft_atoi(av[4]);
	arg->must_eat = -1;
	if (ac == 6)
	{
		arg->must_eat = ft_atoi(av[5]);
		if (arg->must_eat < 0)
			return (0);
	}
	if (arg->philo_num <= 0 || arg->die_time < 0 || \
		arg->eat_time < 0 || arg->sleep_time < 0)
		return (0);
	return (1);
}

int	mutex_init(t_share *share, t_arg *arg)
{
	int	i;

	if (pthread_mutex_init(&(share->print), NULL))
		return (0);
	if (pthread_mutex_init(&(share->lock), NULL))
		return (0);
	i = 0;
	while (i < arg->philo_num)
	{
		if (pthread_mutex_init(&(share->fork[i]), NULL))
			return (0);
		i++;
	}
	share->arg = arg;
	share->end_flag = 0;
	memset(share->f_stat, 0, sizeof(int) * share->arg->philo_num);
	memset(&share->start, 0, sizeof(struct timeval));
	return (1);
}

int	philo_init(t_share *share)
{
	int	i;

	i = 0;
	while (i < share->arg->philo_num)
	{
		share->p[i].share = share;
		pthread_mutex_init(&(share->p[i].lock), 0);
		share->p[i].num = i;
		share->p[i].one_fork = i;
		share->p[i].ano_fork = (i + 1) % share->arg->philo_num;
		memset(&share->p[i].last, 0, sizeof(struct timeval));
		share->p[i].eat_count = 0;
		share->p[i].life_span = share->arg->die_time;
		i++;
	}
	return (1);
}
