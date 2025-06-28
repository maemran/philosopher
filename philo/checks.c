/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:06:14 by maemran           #+#    #+#             */
/*   Updated: 2025/06/28 15:08:23 by maemran          ###   ########.fr       */
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

int    permission_to_print(t_philos* philo)
{
    int res;
    
    pthread_mutex_lock(&philo->data->death);
    res = philo->data->is_dead;
    pthread_mutex_unlock(&philo->data->death);
    return (res);
}

int    death_flag(t_philos* philo)
{
    pthread_mutex_lock(&philo->data->death);
    if (!philo->data->is_dead)
    {
        pthread_mutex_unlock(&philo->data->death);
        return (1);
    }
    philo->data->is_dead = 0;
    pthread_mutex_unlock(&philo->data->death);
    return (0);
}

int    is_all_ate_enough(t_data *data)
{
    int result;
    
    pthread_mutex_lock(&data->finish_mutex);
    result = data->all_ate_enough;
    pthread_mutex_unlock(&data->finish_mutex);
    return (result);
}
