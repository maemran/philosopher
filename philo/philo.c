/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maemran < maemran@student.42amman.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:07:29 by maemran           #+#    #+#             */
/*   Updated: 2025/06/24 17:02:14 by maemran          ###   ########.fr       */
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

int    cuurent_time(void)
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


int take_forks(t_philos *philo, t_data *data)
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
    pthread_mutex_lock(&data->std_out);
    printf("\033[32m[%d] Philosopher %d has taken forks %d.\033[0m\n", 
        cuurent_time() - data->start_time ,philo->id, philo->left_fork + 1);
    printf("\033[32m[%d] Philosopher %d has taken forks %d.\033[0m\n",
        cuurent_time() - data->start_time, philo->id, philo->right_fork + 1);
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
    pthread_mutex_lock(&data->std_out);
    printf("%i,   %i\n", time , data->time_to_die);
    usleep(1000);
    pthread_mutex_unlock(&data->std_out);
    if (time >= data->time_to_die)
    {
        pthread_mutex_lock(&data->std_out);
        printf("\033[34m[%d] Philosopher %d is died.\033[0m\n",
            time, philo->id);
        pthread_mutex_unlock(&data->std_out);
        return (FAILURE);
    }
    return (SUCCESS);
}

void precise_sleep(t_philos *philo, t_data *data, int ms)
{
    int start;
    
    start = cuurent_time();
    while ((cuurent_time() - start) < ms)
    {
        if (death_monitor(philo, data, (cuurent_time() - start)))
        {
            pthread_mutex_lock(&data->death);
            data->is_dead = 0;
            pthread_mutex_unlock(&data->death);
        }
        usleep(100);
    }
    // pthread_mutex_lock(&data->std_out);
    // printf("%i,   %i\n", (cuurent_time() - start), data->time_to_die);
    // pthread_mutex_unlock(&data->std_out);
}


int eating(t_philos *philo, t_data *data)
{
    pthread_mutex_lock(&data->std_out);
    printf("\033[34m[%d] Philosopher %d is eating.\033[0m\n",
        cuurent_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    precise_sleep(philo, data, data->time_to_eat);
    release_forks(philo, data);
    // pthread_mutex_lock(&data->start_time_mutex);
    // data->last_meal = cuurent_time();
    // pthread_mutex_unlock(&data->start_time_mutex);
    return (SUCCESS);
}

int sleeping(t_philos *philo, t_data *data)
{
    pthread_mutex_lock(&data->std_out);
    printf("\033[35m[%d] Philosopher %d is sleeping.\033[0m\n",
        cuurent_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    precise_sleep(philo, data, data->time_to_sleep);
    return (SUCCESS);
}

int thinking(t_philos *philo, t_data *data)
{
    pthread_mutex_lock(&data->std_out);
    printf("\033[36m[%d] Philosopher %d is thinking.\033[0m\n",
        cuurent_time() - data->start_time, philo->id);
    pthread_mutex_unlock(&data->std_out);
    return (SUCCESS);
}

void    *routine(void *arg)
{
    t_philos *philo;
    t_data   *data;

    philo = (t_philos *)arg;
    data = philo->data;
    if (data->start_time == FAILURE)
        return (NULL);
    if (philo->id % 2 == 0)
        usleep(100);
    while (data->is_dead)
    {
        take_forks(philo, data);
        eating(philo, data);
        thinking(philo, data);
        sleeping(philo, data);
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
    data->start_time = cuurent_time();
    while (i < data->philos_num)
    {
        philo[i].data = data;
        pthread_create(&data->threads[i], NULL, routine, &philo[i]);
        i++;
    }
    i = 0;
    while (i < data->philos_num)
    {
        pthread_join(data->threads[i], NULL);
        i++;
    }
    return (SUCCESS);
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