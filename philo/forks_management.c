/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:10:38 by maemran           #+#    #+#             */
/*   Updated: 2025/06/28 15:38:10 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	swap_forks(t_philos *philo, t_data *data)
{
	if (philo->id % 2 != 0)
	{
		pthread_mutex_lock(&data->forks[philo->left_fork]);
		pthread_mutex_lock(&data->forks[philo->right_fork]);
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->right_fork]);
		pthread_mutex_lock(&data->forks[philo->left_fork]);
	}
}

int	taking_fork_by_single_philo(t_philos *philo, t_data	*data)
{
	if (data->philos_num == 1)
	{
		pthread_mutex_lock(&data->std_out);
		printf("[%ld] \033[1;36mPhilosopher %d has taken fork.\033[0m\n",
			current_time() - data->start_time, philo->id);
		pthread_mutex_unlock(&data->std_out);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	take_forks(t_philos *philo, t_data *data)
{
	if (taking_fork_by_single_philo(philo, data))
		return (FAILURE);
	if (is_all_ate_enough(data))
		return (FAILURE);
	pthread_mutex_lock(&data->death);
	if (data->is_dead == 0 || ((current_time()
				- philo->last_meal) >= data->time_to_die))
	{
		pthread_mutex_unlock(&data->death);
		return (FAILURE);
	}
	pthread_mutex_unlock(&data->death);
	swap_forks(philo, data);
	pthread_mutex_lock(&data->death);
	if (data->is_dead == 0)
	{
		release_forks(philo, data);
		pthread_mutex_unlock(&data->death);
		return (FAILURE);
	}
	pthread_mutex_unlock(&data->death);
	print_forks(philo, data);
	return (SUCCESS);
}

int	release_forks(t_philos *philo, t_data *data)
{
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
	return (SUCCESS);
}

void	print_forks(t_philos *philo, t_data *data)
{
	pthread_mutex_lock(&data->std_out);
	if (permission_to_print(philo) && !is_all_ate_enough(data))
	{
		printf("[%ld] \033[1;36mPhilosopher %d has taken fork %d.\033[0m\n",
			current_time() - data->start_time, philo->id, philo->left_fork + 1);
		printf("[%ld] \033[1;36mPhilosopher %d has taken fork %d.\033[0m\n",
			current_time() - data->start_time, philo->id, philo->right_fork
			+ 1);
	}
	pthread_mutex_unlock(&data->std_out);
}
