/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihyjeon <jihyjeon@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:40:12 by jihyjeon          #+#    #+#             */
/*   Updated: 2024/08/24 20:29:02 by jihyjeon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define DIE 0

typedef struct s_arg
{
	int	p_cnt;
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
	int				one;
	int				other;
	int				eating;
	struct timeval	last;
	int				eat_count;
	int				life_span;
}	t_philo;

typedef struct s_share
{
	t_philo			*p;
	t_arg			*arg;
	pthread_t		*tid;
	pthread_mutex_t	*fork;
	pthread_mutex_t	print;
	pthread_mutex_t	lock;
	struct timeval	start;
	int				end_flag;
	int				*f_stat;
}	t_share;

int			valid_input(char **av);
int			set_arg(int ac, char **av, t_arg *arg);
int			mutex_init(t_share *share, t_arg *arg);
int			philo_init(t_share *share);
int			philo(t_share *share);
int			one_philo(t_share *share);
void		*routine(void *philo);
void		take_a_fork(t_philo *p, int num);
void		drop_fork(t_philo *p);
void		eat(t_philo *p);
int			exit_process(t_share *share);
void		print(int status, t_philo *philo, long long time);
long long	timestamp(struct timeval tv);
int			is_alive(t_philo *p, t_arg arg);
void		set_dead(t_share *share);
void		ft_usleep(long long sleep);
int			ft_atoi(const char *str);
int			heap_init(t_share *share, int num);

#endif