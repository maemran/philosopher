/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:45:55 by maemran           #+#    #+#             */
/*   Updated: 2025/06/29 18:11:46 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	eating_process(t_philos *philo, t_data *data)
{
	pthread_mutex_unlock(&data->death);
	pthread_mutex_lock(&data->last_meal_mutex);
	philo->last_meal = current_time();
	pthread_mutex_unlock(&data->last_meal_mutex);
	pthread_mutex_lock(&data->std_out);
	if (permission_to_print(philo) && !is_all_ate_enough(data))
		printf("[%ld] \033[1;95mPhilosopher %d is eating.\033[0m\n",
			current_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->std_out);
	if (precise_sleep(philo, data, data->time_to_eat))
	{
		release_forks(philo, data);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	eating(t_philos *philo, t_data *data)
{
	if (is_all_ate_enough(data))
	{
		release_forks(philo, data);
		return (FAILURE);
	}
	pthread_mutex_lock(&data->death);
	if (data->is_dead == 0 || ((current_time()
				- philo->last_meal) > data->time_to_die))
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

int	sleeping(t_philos *philo, t_data *data)
{
	if (is_dead_flag_check(data))
		return (FAILURE);
	pthread_mutex_lock(&data->std_out);
	if (permission_to_print(philo) && !is_all_ate_enough(data))
		printf("[%ld] \033[33mPhilosopher %d is sleeping.\033[0m\n",
			current_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->std_out);
	precise_sleep(philo, data, data->time_to_sleep);
	return (SUCCESS);
}

int	thinking(t_philos *philo, t_data *data)
{
	if (is_dead_flag_check(data))
		return (FAILURE);
	pthread_mutex_lock(&data->std_out);
	if (permission_to_print(philo) && !is_all_ate_enough(data))
		printf("[%ld] \033[1;34mPhilosopher %d is thinking.\033[0m\n",
			current_time() - data->start_time, philo->id);
	pthread_mutex_unlock(&data->std_out);
	return (SUCCESS);
}

void	*routine(void *arg)
{
	t_philos	*philo;
	t_data		*data;

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
