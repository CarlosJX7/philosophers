/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:41:08 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/16 15:59:00 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo_simulation.h"
#include "../header/simulation_aux.h"

t_bool	ft_philos_full(t_philo *philos)
{
	int	i;
	int	philos_full;

	if (philos->meals_required == -1)
		return (false);
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
		return (true);
	return (false);
}

void	*ft_monitor_thread(void *ptr)
{
	t_philo	*philos;

	philos = (t_philo *)ptr;
	while (true)
	{
		if (ft_starving_check(philos))
			return (NULL);
		if (ft_philos_full(philos) == true)
		{
			pthread_mutex_lock(philos->mutexes.dead_lock);
			*philos->sim_stop_flag = true;
			pthread_mutex_unlock(philos->mutexes.dead_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	ft_philos_routine(t_philo *philo)
{
	t_mutex	*first_fork;
	t_mutex	*second_fork;

	if (philo->total_philos == 1)
	{
		ft_one_philo(philo);
		return ;
	}
	if (philo->mutexes.left_fork < philo->mutexes.right_fork)
	{
		first_fork = philo->mutexes.left_fork;
		second_fork = philo->mutexes.right_fork;
	}
	else
	{
		first_fork = philo->mutexes.right_fork;
		second_fork = philo->mutexes.left_fork;
	}
	ft_eat_sleep_think(philo, first_fork, second_fork);
}

void	*ft_start_philo_thread(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	if (philo->philo_id % 2 == 0)
		ft_sleep_ms(1);
	while (!ft_check_death_flag(philo))
		ft_philos_routine(philo);
	return (NULL);
}

void	ft_start_simulation_threads(t_simul *sim)
{
	pthread_t	monitor_id;
	int			i;
	size_t		start_time;

	start_time = ft_get_time();
	i = 0;
	while (i < sim->philos->total_philos)
	{
		sim->philos[i].times.birth_time = start_time;
		sim->philos[i].times.last_meal = start_time;
		i++;
	}
	i = 0;
	while (i < sim->philos->total_philos)
	{
		if (pthread_create(&sim->philos[i].thread_id, NULL,
				&ft_start_philo_thread, &sim->philos[i]) != 0)
			ft_destroy_mutexes(sim, "Error al crear los hilos de los philos\n",
				sim->philos->total_philos, 1);
		i++;
	}
	if (pthread_create(&monitor_id, NULL, &ft_monitor_thread, sim->philos) != 0)
		ft_destroy_mutexes(sim, "Error al crear el hilo monitor\n",
			sim->philos->total_philos, 1);
	ft_join_threads(monitor_id, sim);
}
