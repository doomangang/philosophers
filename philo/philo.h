/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:40:12 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/16 18:10:32 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

# define IMAX 2147483647
# define IMIN -2147483648

typedef struct s_arg
{
	int	philo_num;
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	must_eat;
}	t_arg;

typedef struct s_share
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	*print;
	long long		start_time;
	bool			end_flag;
}	t_share;

typedef struct s_philo
{
	int			num;
	int			l_fork;
	int			r_fork;
	int			die_time;
	int			eat_count;
	t_arg		*arg;
	t_share		*share;
	pthread_t	*thread;
}	t_philo;

int			valid_input(char **av);
int			set_arg(int ac, char **av, t_arg *arg);
int			mutex_init(t_share *share, int num);
int			philo_init(t_philo *philo, t_arg *arg, t_share *share);
long long	get_time(void);
int			ft_atoi(const char *str);
int	print_error(char *str);

#endif