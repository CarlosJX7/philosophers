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

void ft_philos_finished_eaten(void);
void ft_philos_routine(void);
void ft_start_philos_thread(void);


void ft_monitor_thread(void *ptr)
{
	t_philo *philos;
	int i;

	philos = (t_philo *)ptr;
	i = 0;
	while (i < philos->total_philos)
	{
		if ()
		{
			/* code */
		}
		
		i++;
	}
	
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
				&ft_start_philos_thread, sim->philos))
			ft_destroy_mutexes(sim, "Error al crear los hilos de los philos\n", sim->philos->total_philos, 1);
		i++;
	}
	i = 0;
}

