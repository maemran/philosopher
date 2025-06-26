/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:45:55 by maemran           #+#    #+#             */
/*   Updated: 2025/06/26 19:55:29 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int is_dead_flag_check(t_data *data)
{
    pthread_mutex_lock(&data->death);
    if (data->is_dead == 0)
    {
        pthread_mutex_unlock(&data->death);
        return (FAILURE);
    }
    pthread_mutex_unlock(&data->death);
    return (SUCCESS);
}

int eating(t_philos *philo, t_data *data)
{
    pthread_mutex_lock(&data->death);
    if (data->is_dead == 0
        || ((current_time() - philo->last_meal) > data->time_to_die))
    {
        pthread_mutex_unlock(&data->death);
        release_forks(philo, data);
        return (FAILURE);
    }
    pthread_mutex_unlock(&data->death);
    pthread_mutex_lock(&data->last_meal_mutex);
    philo->last_meal = current_time();
    pthread_mutex_unlock(&data->last_meal_mutex);
    pthread_mutex_lock(&data->std_out);
    if (permission_to_print(philo))
        printf("\033[34m[%ld] Philosopher %d is eating.\033[0m\n",
            current_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    if (precise_sleep(philo, data, data->time_to_eat))
    {
        release_forks(philo, data);
        return (FAILURE);
    }
    (philo->eating_num)++;
    release_forks(philo, data);
    return (SUCCESS);
}

int sleeping(t_philos *philo, t_data *data)
{
    if (is_dead_flag_check(data))
        return (FAILURE);
    pthread_mutex_lock(&data->std_out);
    if (permission_to_print(philo))
        printf("\033[35m[%ld] Philosopher %d is sleeping.\033[0m\n",
            current_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    precise_sleep(philo, data, data->time_to_sleep);
    return (SUCCESS);
}

int thinking(t_philos *philo, t_data *data)
{
    if (is_dead_flag_check(data))
        return (FAILURE);
    pthread_mutex_lock(&data->std_out);
    if (permission_to_print(philo))
        printf("\033[36m[%ld] Philosopher %d is thinking.\033[0m\n",
            current_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    return (SUCCESS);
}

void    *routine(void *arg)
{
    t_philos *philo;
    t_data   *data;

    philo = (t_philos *)arg;
    data = philo->data;
    if (data->start_time == FAILURE)
        return (NULL);
    while (1)
    {
        pthread_mutex_lock(&data->eating);
        if (!(data->they_all_ate) && data->num_of_eat != -2)
        {
            pthread_mutex_unlock(&data->eating);
            break;
        }
        pthread_mutex_unlock(&data->eating);
        if (is_dead_flag_check(data))
            break ;
        if (take_forks(philo, data))
            break ;
        if (eating(philo, data))
            break ;
        if (thinking(philo, data))
            break ;
        if (sleeping(philo, data))
            break ;
    }
    return (NULL);
}
