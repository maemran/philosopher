/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:07:29 by maemran           #+#    #+#             */
/*   Updated: 2025/06/28 14:48:33 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long    current_time(void)
{
    struct timeval	time;
    long current_time;

    if (gettimeofday(&time, NULL) == -1)
    {
        write(2, "gettimeofday() error\n", 22);
        return (FAILURE);
    }
    current_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    return (current_time);
}

void    swap_forks(t_philos *philo, t_data *data)
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

void    print_forks(t_philos *philo, t_data *data)
{
    pthread_mutex_lock(&data->std_out);
    if (permission_to_print(philo) && !is_all_ate_enough(data))
    {
        printf("\033[32m[%ld] Philosopher %d has taken fork %d.\033[0m\n", 
            current_time() - data->start_time ,philo->id, philo->left_fork + 1);
        printf("\033[32m[%ld] Philosopher %d has taken fork %d.\033[0m\n",
            current_time() - data->start_time, philo->id, philo->right_fork + 1);
    }
    pthread_mutex_unlock(&data->std_out);
}

int taking_fork_by_single_philo(t_philos *philo, t_data *data)
{
    if (data->philos_num == 1)
    {
        pthread_mutex_lock(&data->std_out);
        printf("\033[32m[%ld] Philosopher %d has taken fork.\033[0m\n",
            current_time() - data->start_time, philo->id);
        pthread_mutex_unlock(&data->std_out);
        return (FAILURE);
    }
    return (SUCCESS);
}

int take_forks(t_philos *philo, t_data *data)
{
    if (taking_fork_by_single_philo(philo, data))
        return (FAILURE);
    if (is_all_ate_enough(data))
        return (FAILURE);
    pthread_mutex_lock(&data->death);
    if (data->is_dead == 0
        || ((current_time() - philo->last_meal) >= data->time_to_die))
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

int release_forks(t_philos *philo, t_data *data)
{
    pthread_mutex_unlock(&data->forks[philo->left_fork]);
    pthread_mutex_unlock(&data->forks[philo->right_fork]);
    return (SUCCESS);
}

int precise_sleep(t_philos *philo, t_data *data, long ms)
{
    long start;
    
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

int main(int argc, char **argv)
{
    t_data  *data;
    t_philos    *philo;
    
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
    philo = philos_init(data);
    if (!philo)
    {
        clean_all (philo, data, 0);
        return (FAILURE);
    }
    create_threads(philo, data);
    clean_all(philo, data, 1);
    return (0);
}
