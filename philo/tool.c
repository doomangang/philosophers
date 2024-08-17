/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 15:53:53 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/17 18:39:40 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int		sign;
	int		itgr;

	sign = 1;
	itgr = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= 48 && *str <= 57)
	{
		itgr = itgr * 10 + *str - '0';
		str++;
	}
	return (sign * itgr);
}

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	return ((tv.tv_sec * (long long)1000) + (tv.tv_usec / 1000));
}

int	print_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	while (len--)
		write(2, str++, 1);
	return (0);
}

// int	exit_process(t_share *share)
// {

// }

void	print(char *str, t_philo *philo)
{
	int	time;

	time = get_time() - philo->share->start_time;
	pthread_mutex_lock(&(philo->share->print));
	if (!strcmp(str, "fork"))
		printf("%d %d has taken a fork\n", time, philo->num);
	if (!strcmp(str, "eat"))
		printf("%d %d is eating\n", time, philo->num);
	if (!strcmp(str, "sleep"))
		printf("%d %d is sleeping\n", time, philo->num);
	if (!strcmp(str, "think"))
		printf("%d %d is thinking\n", time, philo->num);
	if (!strcmp(str, "die"))
		printf("%d %d died\n", time, philo->num);
	pthread_mutex_unlock(&(philo->share->print));
}
