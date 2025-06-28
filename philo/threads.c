/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:14:14 by maemran           #+#    #+#             */
/*   Updated: 2025/06/28 15:14:53 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_death(t_philos *philo, int i)
{
    pthread_mutex_lock(&philo->data->std_out);
    if (!death_flag(philo))
        printf("\033[34m[%ld] Philosopher %d is dead.\033[0m\n",
            current_time() - philo->data->start_time, philo[i].id);
    pthread_mutex_unlock(&philo->data->std_out);
}

void    *death_monitor(void *arg)
{
    t_philos *philo;
    long time;
    int i;

    philo = (t_philos *)arg;
    while (1)
    {
        i = 0;
        while (i < philo->data->philos_num)
        {
            if (is_all_ate_enough(philo->data))
                return (NULL);
            pthread_mutex_lock(&philo->data->last_meal_mutex);
            time = current_time() - philo[i].last_meal;
            pthread_mutex_unlock(&philo->data->last_meal_mutex);
            if (time > philo->data->time_to_die)
            {
                print_death(philo, i);
                return (NULL);
            }
            i++;
        }
        usleep(500);   
    }
    return (NULL);
}

int creat_thread_fail(t_data *data)
{
    printf("\033[31mError: Failed to create thread.\033[0m\n");
    free(data->threads);
    return (FAILURE);
}

void thread_join(t_data *data)
{
    int i;

    i = 0;
    pthread_join(data->monitor_thread, NULL);
    while (i < data->philos_num)
    {
        pthread_join(data->threads[i], NULL);
        i++;
    }
}

int    create_threads(t_philos *philo, t_data *data)
{
    int i;

    i = 0;
    
    data->threads = malloc(sizeof(pthread_t) * data->philos_num);
    if (!data->threads)
        return (FAILURE);
    data->start_time = current_time();
    while (i < data->philos_num)
    {
        philo[i].data = data;
        philo[i].last_meal = data->start_time;
        if (pthread_create(&data->threads[i], NULL, routine, &philo[i]) != 0)
            return (creat_thread_fail(data));
        i++;
    }
    if (pthread_create(&data->monitor_thread, NULL, death_monitor, philo) != 0)
        return (creat_thread_fail(data));
    thread_join(data); 
    return (SUCCESS);
}
