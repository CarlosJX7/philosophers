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
		pthread_mutex_lock(philos[i].mutexes.meal_lock);
		if (philos[i].meals_eaten >= philos->meals_required)
			philos_full++;
		pthread_mutex_unlock(philos[i].mutexes.meal_lock);
		i++;
	}
	if (philos_full == philos->total_philos)
	{
		pthread_mutex_lock(philos->mutexes.write_lock);	
		return (1);
	}
	return (0);
}

void *ft_monitor_thread(void *ptr)
{
	t_philo *philos;
	int i;

	philos = (t_philo *)ptr;
	i = 0;
	while (true)
	{
		i = 0;
		while (i < philos->total_philos)
		{
			pthread_mutex_lock(philos[i].mutexes.meal_lock);
			if (ft_get_time() - philos[i].times.last_meal > philos[i].times.tto_starve)
			{
				pthread_mutex_unlock(philos[i].mutexes.meal_lock);
				ft_print_status(&philos[i], "died");
				pthread_mutex_lock(philos->mutexes.write_lock);
				return NULL;
			}
			pthread_mutex_unlock(philos[i].mutexes.meal_lock);
			i++;
		}
		if (ft_philos_full(philos) == 1)
			return NULL;
		usleep(1000); // Reducir carga de CPU
	}
	return NULL; //¿?
}

void ft_philos_routine(t_philo *philo)
{
	t_mutex *first_fork;
	t_mutex *second_fork;

	ft_one_philo(philo);
		return ;
	/*
	{
		pthread_mutex_lock(philo->mutexes.left_fork);
		ft_print_status(philo, "has taken a fork");
		pthread_mutex_unlock(philo->mutexes.left_fork);
			while (true)
				ft_usleep(1000);
		return ;
	}
	*/
	if (philo->philo_id == philo->total_philos)
	{
		first_fork = philo->mutexes.right_fork;
		second_fork = philo->mutexes.left_fork;
	}else
	{
		first_fork = philo->mutexes.left_fork;
		second_fork = philo->mutexes.right_fork;
	}
	pthread_mutex_lock(first_fork);
	ft_print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	ft_print_status(philo, "has taken a fork");
	
	ft_print_status(philo, "is eating");// fuera del mutex lock para mejor sincronizacion
	pthread_mutex_lock(philo->mutexes.meal_lock);
	philo->times.last_meal = ft_get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->mutexes.meal_lock);
	
	
	ft_usleep(philo->times.tto_eat);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
	ft_print_status(philo, "is sleeping");
	ft_usleep(philo->times.tto_sleep);
	ft_print_status(philo, "is thinking");

}

void *ft_start_philo_thread(void *ptr)
{
	t_philo *philo;
	size_t current_time;
	//sleep¿?
	philo = (t_philo *)ptr;
	if (philo->philo_id % 2 == 0)
		ft_usleep(1);
	current_time = ft_get_time();
	philo->times.birth_time = current_time;
	pthread_mutex_lock(philo->mutexes.meal_lock);
	philo->times.last_meal = current_time;
	pthread_mutex_unlock(philo->mutexes.meal_lock);
	while (true)
		ft_philos_routine(philo);
}

void ft_start_simulation_threads(t_simulation *sim)
{
	t_id monitor_id;
	int i;

	if (pthread_create(&monitor_id, NULL, &ft_monitor_thread, sim->philos) != 0)
		ft_destroy_mutexes(sim, "Error al crear el hilo monitor\n", sim->philos->total_philos, 1);
	i = 0;
	while (i < sim->philos->total_philos)
	{
		if (pthread_create(&sim->philos[i].thread_id, NULL,
				&ft_start_philo_thread, &sim->philos[i]) != 0)
			ft_destroy_mutexes(sim, "Error al crear los hilos de los philos\n", sim->philos->total_philos, 1);
		i++;
	}
	if (pthread_join(monitor_id, NULL) != 0)
	ft_destroy_mutexes(sim, "Error al hacer join en la simulacion\n", sim->philos->total_philos, 1);
	i = 0;
	while (i < sim->philos->total_philos)
	{
		if (pthread_detach(sim->philos[i].thread_id))
			ft_destroy_mutexes(sim, "Error en detach los philos\n", sim->philos->total_philos, 1);
		i++;
	}
}
