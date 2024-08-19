/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:40:12 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/19 18:05:20 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_arg
{
	int	philo_num;
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	must_eat;
}	t_arg;

typedef struct s_philo
{
	struct s_share	*share;
	pthread_mutex_t	lock;
	int				num;
	int				l_fork;
	int				r_fork;
	int				eating;
	long long		die_when;
	int				eat_count;
}	t_philo;

typedef struct s_share
{
	t_philo			*philo;
	t_arg			*arg;
	pthread_t		*tid;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print;
	pthread_mutex_t	lock;
	long long		start_time;
	int				end_flag;
}	t_share;

int			valid_input(char **av);
int			set_arg(int ac, char **av, t_arg *arg);
int			mutex_init(t_share *share, t_arg *arg);
int			philo_init(t_share *share);
int			philo(t_share *share);
int			one_philo(t_share *share);
void		*routine(void *philo);
void		*monitor(void *share);
void		eat(t_philo *p);
int			exit_process(t_share *share);
void		print(char *str, t_philo *philo);
long long	get_time(void);
int			ft_atoi(const char *str);
int			print_error(char *str);

#endif