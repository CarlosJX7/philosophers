/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:41:08 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/12 12:41:10 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_simulation.h"

void ft_philos_routine(void);


int ft_philos_full(t_philo *philos)
{
	int i;
	int philos_full;

	if (philos->meals_required == -1)
		return (0);
	i = 0;
	philos_full = 0; 
	while (i < philos->total_philos)
	{
		pthread_mutex_lock(philos->mutexes.meal_lock);
		if (philos[i].meals_eaten >= philos->meals_required)
			philos_full++;
		pthread_mutex_unlock(philos->mutexes.meal_lock);
		i++;
	}
	if (philos_full == philos->total_philos)
		return (1);
	return (0);
}

void ft_monitor_thread(void *ptr)
{
	t_philo *philos;
	int i;

	philos = (t_philo *)ptr;
	i = 0;
	while (true)
	{
		while (i < philos->total_philos)
		{
			pthread_mutex_lock(philos[i].mutexes.meal_lock);
			if (ft_get_time() - philos[i].times.last_meal > philos[i].times.tto_starve)
			{
				pthread_mutex_unlock(philos[i].mutexes.meal_lock);
				ft_print_status(&philos[i], "died");
				return ;
			}
			pthread_mutex_unlock(philos[i].mutexes.meal_lock);
			i++;
		}
		if (ft_philos_full(philos) == 1)
			return ;
	}
	return ; //¿?
}

void ft_start_philo_thread(void *ptr)
{
	t_philo *philos;

	philos = (t_philo  *)ptr;


}

void ft_start_simulation_threads(t_simulation *sim)
{
	t_id monitor_id;
	int i;

	i = 0;
	if (pthread_create(&monitor_id, NULL, &ft_monitor_thread, sim->philos) != 0)
		ft_destroy_mutexes(sim, "Error al crear el hilo monitor\n", sim->philos->total_philos, 1);
	while (i < sim->philos->total_philos)
	{
		if (pthread_create(&sim->philos[i].thread_id, NULL,
				&ft_start_philo_thread, sim->philos))
			ft_destroy_mutexes(sim, "Error al crear los hilos de los philos\n", sim->philos->total_philos, 1);
		i++;
	}
	i = 0;
}

