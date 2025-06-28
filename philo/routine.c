/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:45:55 by maemran           #+#    #+#             */
/*   Updated: 2025/06/28 14:26:51 by maemran          ###   ########.fr       */
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

void    meals_num_check(t_philos *philo, t_data *data)
{
    philo->meals_num++;
    if (data->num_of_eat > 0 &&
        philo->meals_num == data->num_of_eat)
    {
        pthread_mutex_lock(&data->finish_mutex);
        data->philos_finished_meals++;
        if (data->philos_finished_meals == data->philos_num)
            data->all_ate_enough = 1;
        pthread_mutex_unlock(&data->finish_mutex);
    }
}

int eating_process(t_philos *philo, t_data *data)
{
    pthread_mutex_unlock(&data->death);
    pthread_mutex_lock(&data->last_meal_mutex);
    philo->last_meal = current_time();
    pthread_mutex_unlock(&data->last_meal_mutex);
    pthread_mutex_lock(&data->std_out);
    if (permission_to_print(philo) && !is_all_ate_enough(data))
        printf("\033[34m[%ld] Philosopher %d is eating.\033[0m\n",
            current_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    if (precise_sleep(philo, data, data->time_to_eat))
    {
        release_forks(philo, data);
        return (FAILURE);
    }
    return (SUCCESS);
}

int eating(t_philos *philo, t_data *data)
{
    if (is_all_ate_enough(data))
    {
        release_forks(philo, data);
        return (FAILURE);
    }
    pthread_mutex_lock(&data->death);
    if (data->is_dead == 0
        || ((current_time() - philo->last_meal) > data->time_to_die))
    {
        pthread_mutex_unlock(&data->death);
        release_forks(philo, data);
        return (FAILURE);
    }
    if (eating_process(philo, data))
        return (FAILURE);
    meals_num_check(philo, data);
    release_forks(philo, data);
    if (is_all_ate_enough(data))
        return (FAILURE);
    return (SUCCESS);
}

int sleeping(t_philos *philo, t_data *data)
{
    if (is_dead_flag_check(data))
        return (FAILURE);
    pthread_mutex_lock(&data->std_out);
    if (permission_to_print(philo) && !is_all_ate_enough(data))
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
    if (permission_to_print(philo) && !is_all_ate_enough(data))
        printf("\033[36m[%ld] Philosopher %d is thinking.\033[0m\n",
            current_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    return (SUCCESS);
}

int    is_all_ate_enough(t_data *data)
{
    int result;
    
    pthread_mutex_lock(&data->finish_mutex);
    result = data->all_ate_enough;
    pthread_mutex_unlock(&data->finish_mutex);
    return (result);
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
        if (is_dead_flag_check(data) || is_all_ate_enough(data))
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
