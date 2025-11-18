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

void	ft_print_error(char *message, int exit_code)
{
	if (message)
		write(2, message, ft_strlen(message));
	exit(exit_code);
}

void	ft_destroy_mutexes(t_simul *sim, char *text, int n_philos, int ext_cod)
{
	while (n_philos > 0)
	{
		pthread_mutex_destroy(&sim->forks[n_philos - 1]);
		n_philos--;
	}
	pthread_mutex_destroy(&sim->meal_lock);
	pthread_mutex_destroy(&sim->write_lock);
	pthread_mutex_destroy(&sim->dead_lock);
	ft_print_error(text, ext_cod);
}

size_t	ft_get_time(void)
{
	t_timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_print_error("Error en ft_get_time\n", 1);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_print_status(t_philo *philo, char *status)
{
	size_t	time;
	t_bool	is_died;

	is_died = (status[0] == 'd' && status[1] == 'i'
			&& status[2] == 'e' && status[3] == 'd');
	pthread_mutex_lock(philo->mutexes.write_lock);
	if (!ft_check_death_flag(philo) || is_died)
	{
		time = ft_get_time() - philo->times.birth_time;
		printf("[%ld] %d %s\n", time, philo->philo_id, status);
	}
	pthread_mutex_unlock(philo->mutexes.write_lock);
}

void	ft_sleep_ms(size_t time_ms)
{
	size_t	start;

	start = ft_get_time();
	while (ft_get_time() - start < time_ms)
		usleep(500);
}
