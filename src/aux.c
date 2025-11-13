/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:16:50 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/13 17:16:54 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/aux.h"
#include "../header/utils.h"

void ft_print_error(char *message, int exit_code)
{
	if (message)
		write(2, message, ft_strlen(message));
	exit(exit_code);
}

void ft_destroy_mutexes(t_simulation *sim, char *message, int n_philos, int exit_code)
{
	while (n_philos > 0) //bucle para destruir los mutexes de los tenedores
	{
		pthread_mutex_destroy(&sim->forks[n_philos - 1]);
		n_philos--;
	}
	pthread_mutex_destroy(&sim->meal_lock);
	pthread_mutex_destroy(&sim->write_lock);
	ft_print_error(message, exit_code);
}

size_t ft_get_time(void)
{
	t_timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_print_error("Error en ft_get_time\n", 1);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void ft_print_status(t_philo *philo, char *status)
{
	size_t time;

	time = ft_get_time() - philo->times.birth_time;
	pthread_mutex_lock(philo->mutexes.write_lock);
	printf("[%ld] %d %s\n", time, philo->philo_id, status);
}

void ft_usleep(size_t time_ms)
{
	size_t start; 

	start = ft_get_time();
	while (ft_get_time() - start < time_ms)
		usleep(500);
}