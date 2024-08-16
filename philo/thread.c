/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 18:12:06 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/16 21:42:41 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo(t_share *share)
{
	share->start_time = get_time();
	
}

int	one_philo(t_share *share)
{
	share->start_time = get_time();
	if (pthread_create(&share->tid[0], 0, &routine, &share->philo[0]));
		return (0);
	pthread_detach(share->tid[0]);
	exit_process(share);
	return (1);
}

void	*routine(void *philo)
{

}

void	*monitor(void *share)
{

}

int	exit_process(t_share *share)
{

}
