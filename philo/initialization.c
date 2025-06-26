/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:51:00 by maemran           #+#    #+#             */
/*   Updated: 2025/06/25 22:12:14 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int args_check(char *str)
{
    int length;

    length = 0;
    while (str[length])
    {
        if (str[0] == '+' && str[1] != '\0')
        {
            length++;
            continue;
        }
        if (!(str[length] >= '0' && str[length] <= '9'))
            return(-1);
        length++;
    }
    return (0);
}

int ft_atoi(char *str)
{
    int length;
    int res;
    
    length = 0;
    res = 0;
    if (args_check(str) == -1)
        return(-1);
    if (str[0] == '+')
        length = 1;
    while (str[length])
    {
        res = (res * 10) + (str[length] - '0');
		length++;
    }
    return (res);
}

int forks_init(t_data *data)
{
    int i;

    i = 0;
    data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_num);
    if (!data->forks)
        return (FAILURE);
    while(i < data->philos_num)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        i++;
    }
    return(SUCCESS);
}

int    data_init(t_data *data, char **argv, int argc)
{
    data->philos_num = ft_atoi(argv[1]);
    data->time_to_die = ft_atoi(argv[2]);
    data->time_to_eat = ft_atoi(argv[3]);
    data->time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        data->num_of_eat = ft_atoi(argv[5]);
    else
        data->num_of_eat = -2;
    if (data->philos_num == -1 || data->time_to_die == -1
        || data->time_to_eat == -1 || data->time_to_sleep == -1
        || data->num_of_eat == -1)
    {
        printf("\033[31mIts a non numric value.\n\033[0m");
        return(FAILURE);
    }
    data->is_dead = 1;
    if (forks_init(data))
        return (FAILURE);
    pthread_mutex_init(&data->std_out, NULL);
    pthread_mutex_init(&data->death, NULL);
    pthread_mutex_init(&data->last_meal_mutex, NULL);
    return(SUCCESS);
}

t_philos    *philos_init(t_data  *data)
{
    int i;
    t_philos    *philos;
    
    i = 0;
    philos = malloc(sizeof(t_philos) * data->philos_num);
    if (!philos)
        return (NULL);
    while (i < data->philos_num)
    {
        philos[i].id = i + 1;
        philos[i].left_fork = i;
        philos[i].right_fork = (i + 1) % data->philos_num;
        i++;
    }
    return (philos);
}