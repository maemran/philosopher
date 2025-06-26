/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:07:29 by maemran           #+#    #+#             */
/*   Updated: 2025/06/26 00:01:04 by maemran          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int    current_time(void)
{
    struct timeval	time;
    int current_time;

    if (gettimeofday(&time, NULL) == -1)
    {
        write(2, "gettimeofday() error\n", 22);
        return (FAILURE);
    }
    current_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
    return (current_time);
}

int take_forks(t_philos *philo, t_data *data)
{
    // pthread_mutex_lock(&data->death);
    // if (data->is_dead == 0)
    // {
    //     pthread_mutex_unlock(&data->death);
    //     return (FAILURE);
    // }
    // pthread_mutex_unlock(&data->death);
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
    pthread_mutex_lock(&data->death);
    if (data->is_dead == 0)
    {
        release_forks(philo, data);
        pthread_mutex_unlock(&data->death);
        return (FAILURE);
    }
    pthread_mutex_unlock(&data->death);
    pthread_mutex_lock(&data->std_out);
    printf("\033[32m[%d] Philosopher %d has taken forks %d.\033[0m\n", 
        current_time() - data->start_time ,philo->id, philo->left_fork + 1);
    printf("\033[32m[%d] Philosopher %d has taken forks %d.\033[0m\n",
        current_time() - data->start_time, philo->id, philo->right_fork + 1);
    pthread_mutex_unlock(&data->std_out);
    return (SUCCESS);
}

int release_forks(t_philos *philo, t_data *data)
{
    pthread_mutex_unlock(&data->forks[philo->left_fork]);
    pthread_mutex_unlock(&data->forks[philo->right_fork]);
    return (SUCCESS);
}

int death_monitor(t_philos *philo, t_data *data, int time)
{
    (void)philo;
    // pthread_mutex_lock(&data->std_out);
    // printf("%i,   %i\n", time , data->time_to_die);
    // usleep(1000);
    // pthread_mutex_unlock(&data->std_out);
    if (time >= data->time_to_die)
    {
        // pthread_mutex_lock(&data->std_out);
        // printf("\033[34m[%d] Philosopher %d is died.\033[0m\n",
        //     time, philo->id);
        // pthread_mutex_unlock(&data->std_out);
        return (FAILURE);
    }
    return (SUCCESS);
}

int precise_sleep(t_philos *philo, t_data *data, int ms)
{
    int start;
    
    (void)philo;
    start = current_time();
    while ((current_time() - start) < ms)
    {
        pthread_mutex_lock(&data->death);
        if (data->is_dead == 0)
        {
            pthread_mutex_unlock(&data->death);
            return (FAILURE);
        }
        pthread_mutex_unlock(&data->death);
        usleep(5);
    }
    return (SUCCESS);
}

void    *monitor_routine(void *arg)
{
    t_philos *philo;
    int time;
    int i;

    philo = (t_philos *)arg;
    while (1)
    {
        i = 0;
        while (i < philo->data->philos_num)
        {
            pthread_mutex_lock(&philo->data->last_meal_mutex);
            time = current_time() - philo[i].last_meal;
            pthread_mutex_unlock(&philo->data->last_meal_mutex);
            if (time >= philo->data->time_to_die)
            {
                pthread_mutex_lock(&philo->data->std_out);
                printf("\033[34m[%d] Philosopher %d is dead.\033[0m\n",
                    current_time() - philo->data->start_time, philo[i].id);
                pthread_mutex_unlock(&philo->data->std_out);
                pthread_mutex_lock(&philo->data->death);
                philo->data->is_dead = 0;
                pthread_mutex_unlock(&philo->data->death);
                return (NULL);
            }
            i++;
        }
        usleep(1000);   
    }
    return (NULL);
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
        pthread_create(&data->threads[i], NULL, routine, &philo[i]);
        i++;
    }
    pthread_create(&data->monitor_thread, NULL, monitor_routine, philo);
    i = 0;
    while (i < data->philos_num)
    {
        pthread_join(data->threads[i], NULL);
        i++;
    }
    pthread_join(data->monitor_thread, NULL);
    return (SUCCESS);
}


int main(int argc, char **argv)// philo_num    td    te   ts  op
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
        return (FAILURE);
    philo = philos_init(data);
    if (!philo)
        return (FAILURE);
    create_threads(philo, data);
    
    // int i = 0;
    
    // while (i < data->philos_num)
    // {
    //     printf("philo_id: %i\n", philo[i].id);
    //     i++;
    // }

}