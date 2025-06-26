/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:07:34 by maemran           #+#    #+#             */
/*   Updated: 2025/06/26 19:32:03 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# define BOLD "\033[1m"
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define RESET "\033[0m"

# define SUCCESS 0
# define FAILURE 1

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_data
{
    int philos_num;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    long start_time;
    int num_of_eat;
    int is_dead;
    int they_all_ate;
    pthread_t *threads;
    pthread_t monitor_thread;
    pthread_t eating_monitor;
    pthread_mutex_t *forks;
    pthread_mutex_t std_out;
    pthread_mutex_t death;
    pthread_mutex_t eating;
    pthread_mutex_t last_meal_mutex;
}   t_data;

typedef struct s_philos
{
    int id;
    int left_fork;
    int right_fork;
    int  eating_num;
    long last_meal;
    t_data  *data;
}   t_philos;


int    permission_to_print(t_philos* philo);
void    *routine(void *arg);
long    current_time(void);
int take_forks(t_philos *philo, t_data *data);
int release_forks(t_philos *philo, t_data *data);
t_philos    *philos_init(t_data  *data);
int    data_init(t_data *data, char **argv, int argc);
int precise_sleep(t_philos *philo, t_data *data, long ms);
int is_dead_flag_check(t_data *data);

#endif 
