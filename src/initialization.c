/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 15:40:32 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/06 15:40:34 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/initialization.h"
#include "../header/philo.h"
#include "../header/utils.h"
#include "../header/aux.h"

void ft_init_simulation(t_simulation *sim, t_philo *philos, t_mutex *forks)
{
	sim->forks = forks;
	sim->philosophers = philos;
	if (pthread_mutex_init(&sim->meal_lock, NULL) != 0
	|| pthread_mutex_init(&sim->write_lock, NULL) != 0)
	ft_destroy_mutexes(sim, "Error al iniciar mutexes globales\n", 0, 1);	
}

void ft_init_forks(t_simulation *sim, int n_philos)
{
	int i;

	i = 0;

	while (i < n_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0) // necesario & porque pasamos los forks de forma unitaria
			ft_destroy_mutexes(sim, "Errior al iniciar mutexes de tenedores\n", i, 1);
		i++;	
	}
}

void ft_init_philosophers(t_simulation *sim, t_philo *philos, t_mutex *forks, char **argv)
{
	int i;

	i = 0;
	while (i < ft_atoi(argv[1]))
	{
		philos[i].philosopher_id = i;
		philos[i].times.time_to_die = ft_atoi(argv[2]);
		philos[i].times.time_to_eat = ft_atoi(argv[3]);
		philos[i].times.time_to_sleep = ft_atoi(argv[4]);
		philos[i].times.last_meal_time = get_current_time();
		philos[i].times.birth_time = get_current_time();
		philos[i].meals_required = -1;
		if (argv[5])
			philos[i].meals_required = ft_atoi(argv[5]);
		philos[i].meals_eaten = 0;
		philos[i].total_philosophers = ft_atoi(argv[1]);
		philos[i].mutexes.left_fork = &forks[i]; //left fork es un puntero al mutex del tenedor izquierdo
		if (i == 0)
			philos[i].mutexes.right_fork = &forks[philos[i].total_philosophers - 1];
		else
			philos[i].mutexes.right_fork = &forks[i - 1];
		philos[i].mutexes.write_lock = &sim->write_lock;
		philos[i].mutexes.meal_lock = &sim->meal_lock;
		i++;
	}
}


void ft_init_engine()
{

}