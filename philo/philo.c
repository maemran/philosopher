/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:07:29 by maemran           #+#    #+#             */
/*   Updated: 2025/06/28 15:15:04 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void clean_all(t_philos *philo, t_data *data, int type)
{
    int i;

    i = 0;
    while (i < data->philos_num)
    {
        pthread_mutex_destroy(&data->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&data->std_out);
    pthread_mutex_destroy(&data->death);
    pthread_mutex_destroy(&data->meal_mutex);
    pthread_mutex_destroy(&data->finish_mutex);
    pthread_mutex_destroy(&data->last_meal_mutex);
    free(data->forks);
    if (type == 1)
    {
        free(philo);
        free(data->threads);
    }
    free(data);
}

int philo_brain(t_data *data)
{
    t_philos    *philo;
    
    philo = philos_init(data);
    if (!philo)
    {
        clean_all (philo, data, 0);
        return (FAILURE);
    }
    if (create_threads(philo, data))
    {
        clean_all(philo, data, 0);
        free(philo);
        return (FAILURE);
    }
    clean_all(philo, data, 1);
    return (SUCCESS);
}

int main(int argc, char **argv)
{
    t_data  *data;
    
    if (argc != 5 && argc != 6)
    {
        printf("%s\n", "\033[31mThe number of args is wrong.\033[0m");
        return (FAILURE);
    }
    data = malloc(sizeof(t_data));
    if (!data)
        return (FAILURE);
    if (data_init(data, argv, argc))
    {
        free (data);
        return (FAILURE);
    }
    if (philo_brain(data))
        return (FAILURE);
    return (0);
}
