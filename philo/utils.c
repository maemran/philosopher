/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 14:55:17 by maemran           #+#    #+#             */
/*   Updated: 2025/06/28 15:22:36 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_time(void)
{
	struct timeval	time;
	long			current_time;

	if (gettimeofday(&time, NULL) == -1)
	{
		write(2, "gettimeofday() error\n", 22);
		return (FAILURE);
	}
	current_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (current_time);
}

int	args_check(char *str)
{
	int	length;

	length = 0;
	while (str[length])
	{
		if (str[0] == '+' && str[1] != '\0')
		{
			length++;
			continue ;
		}
		if (!(str[length] >= '0' && str[length] <= '9'))
			return (-1);
		length++;
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int	length;
	int	res;

	length = 0;
	res = 0;
	if (args_check(str) == -1)
		return (-1);
	if (str[0] == '+')
		length = 1;
	while (str[length])
	{
		res = (res * 10) + (str[length] - '0');
		length++;
	}
	return (res);
}

int	destroy_mutex(t_data *data)
{
	pthread_mutex_destroy(&data->meal_mutex);
	pthread_mutex_destroy(&data->finish_mutex);
	pthread_mutex_destroy(&data->std_out);
	pthread_mutex_destroy(&data->death);
	return (FAILURE);
}

int	precise_sleep(t_philos *philo, t_data *data, long ms)
{
	long	start;

	(void)philo;
	start = current_time();
	while ((current_time() - start) < ms)
	{
		if (is_dead_flag_check(data))
			return (FAILURE);
		usleep(100);
	}
	return (SUCCESS);
}
