/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:51:00 by maemran           #+#    #+#             */
/*   Updated: 2025/06/28 20:35:00 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	forks_init(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_num);
	if (!data->forks)
		return (FAILURE);
	while (i < data->philos_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			free(data->forks);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
		return (FAILURE);
	if (pthread_mutex_init(&data->finish_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->meal_mutex);
		return (FAILURE);
	}
	if (pthread_mutex_init(&data->std_out, NULL) != 0)
	{
		pthread_mutex_destroy(&data->meal_mutex);
		pthread_mutex_destroy(&data->finish_mutex);
		return (FAILURE);
	}
	if (pthread_mutex_init(&data->death, NULL) != 0)
	{
		pthread_mutex_destroy(&data->meal_mutex);
		pthread_mutex_destroy(&data->finish_mutex);
		pthread_mutex_destroy(&data->std_out);
		return (FAILURE);
	}
	if (pthread_mutex_init(&data->last_meal_mutex, NULL) != 0)
		return (destroy_mutex(data));
	return (SUCCESS);
}

void	init_args(t_data *data, char **argv, int argc)
{
	data->philos_num = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->num_of_eat = ft_atoi(argv[5]);
	else
		data->num_of_eat = -2;
}

int	data_init(t_data *data, char **argv, int argc)
{
	init_args(data, argv, argc);
	if (data->philos_num == -1 || data->philos_num == 0
		|| data->time_to_die == -1 || data->time_to_eat == -1
		|| data->time_to_sleep == -1 || data->num_of_eat == -1
		|| data->num_of_eat == 0)
	{
		printf("\033[31mIts a non numeric value.\n\033[0m");
		return (FAILURE);
	}
	data->is_dead = 1;
	data->philos_finished_meals = 0;
	data->all_ate_enough = 0;
	if (init_mutexes(data))
		return (FAILURE);
	if (forks_init(data))
	{
		pthread_mutex_destroy(&data->last_meal_mutex);
		return (destroy_mutex(data));
	}
	return (SUCCESS);
}

t_philos	*philos_init(t_data *data)
{
	int			i;
	t_philos	*philos;

	i = 0;
	philos = malloc(sizeof(t_philos) * data->philos_num);
	if (!philos)
		return (NULL);
	while (i < data->philos_num)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = i;
		philos[i].meals_num = 0;
		philos[i].right_fork = (i + 1) % data->philos_num;
		i++;
	}
	return (philos);
}
