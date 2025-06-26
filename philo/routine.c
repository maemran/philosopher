/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:45:55 by maemran           #+#    #+#             */
/*   Updated: 2025/06/25 23:58:41 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int eating(t_philos *philo, t_data *data)
{
//     pthread_mutex_lock(&data->death);
//     if (data->is_dead == 0)
//     {
//         pthread_mutex_unlock(&data->death);
//         release_forks(philo, data);
//         return (FAILURE);
//     }
//     pthread_mutex_unlock(&data->death);
    pthread_mutex_lock(&data->std_out);
    printf("\033[34m[%d] Philosopher %d is eating.\033[0m\n",
        current_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    if (precise_sleep(philo, data, data->time_to_eat))
    {
        release_forks(philo, data);
        return (FAILURE);
    }
    release_forks(philo, data);
    pthread_mutex_lock(&data->last_meal_mutex);
    philo->last_meal = current_time();
    pthread_mutex_unlock(&data->last_meal_mutex);
    return (SUCCESS);
}

int sleeping(t_philos *philo, t_data *data)
{
    // pthread_mutex_lock(&data->death);
    // if (data->is_dead == 0)
    // {
    //     pthread_mutex_unlock(&data->death);
    //     return (FAILURE);
    // }
    // pthread_mutex_unlock(&data->death);
    pthread_mutex_lock(&data->std_out);
    printf("\033[35m[%d] Philosopher %d is sleeping.\033[0m\n",
        current_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    if (precise_sleep(philo, data, data->time_to_sleep))
        return (FAILURE);
    return (SUCCESS);
}

int thinking(t_philos *philo, t_data *data)
{
    // pthread_mutex_lock(&data->death);
    // if (data->is_dead == 0)
    // {
    //     pthread_mutex_unlock(&data->death);
    //     return (FAILURE);
    // }
    // pthread_mutex_unlock(&data->death);
    pthread_mutex_lock(&data->std_out);
    printf("\033[36m[%d] Philosopher %d is thinking.\033[0m\n",
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
    if (philo->id % 2 == 0)
        usleep(100);
    while (1)
    {
        pthread_mutex_lock(&data->death);
        if (data->is_dead == 0)
        {
            pthread_mutex_unlock(&data->death);
            break;
        }
        pthread_mutex_unlock(&data->death);
        if (take_forks(philo, data))
            break;
        if (eating(philo, data))
            break;
        if (thinking(philo, data))
            break;
        if (sleeping(philo, data))
            break;
    }
    return (NULL);
}
