/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:07:34 by maemran           #+#    #+#             */
/*   Updated: 2025/06/24 16:41:27 by maemran          ###   ########.fr       */
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
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int start_time;
    int num_of_eat;
    int last_meal;
    int is_dead;
    pthread_t *threads;
    pthread_mutex_t *forks;
    pthread_mutex_t std_out;
    pthread_mutex_t death;
}   t_data;

typedef struct s_philos
{
    int id;
    int left_fork;
    int right_fork;
    t_data  *data;
}   t_philos;


#endif 
