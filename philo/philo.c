/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:07:29 by maemran           #+#    #+#             */
/*   Updated: 2025/06/09 19:50:06 by maemran          ###   ########.fr       */
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
    data->fork_num = malloc(sizeof(int) * data->philos_num);
    if (!data->fork_num)
        return (FAILURE);
    data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_num);
    if (!data->forks)
        return (FAILURE);
    while(i < data->philos_num)
    {
        data->fork_num[i] = 0;
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
    if (forks_init(data))
        return (FAILURE);
    pthread_mutex_init(&data->std_out, NULL);
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
        i++;
    }
    return (philos);
}

int main(int argc, char **argv)// philo_num    td    te   ts  op
{
    t_data  *data;
    t_philos    *philo;
    
    if (argc != 5 && argc != 6)
    {
        printf("%s\n", "\033[31mThe number of args is wrong.\033[0m");
        exit(FAILURE);
    }
    data = malloc(sizeof(t_data));
    if (!data)
        exit (FAILURE);
    if (data_init(data, argv, argc))
        exit(FAILURE);
    philo = philos_init(data);
    int i = 0;
    
    while (i < data->philos_num)
    {
        printf("philo_id: %i\n", philo[i].id);
        i++;
    }
}