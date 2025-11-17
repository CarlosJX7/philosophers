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

void	ft_init_simulation(t_simul *sim, t_philo *philos, t_mutex *forks)
{
	sim->forks = forks;
	sim->philos = philos;
	sim->sim_stop_flag = false;
	if (pthread_mutex_init(&sim->meal_lock, NULL) != 0
		|| pthread_mutex_init(&sim->write_lock, NULL) != 0
		|| pthread_mutex_init(&sim->dead_lock, NULL) != 0)
		ft_destroy_mutexes(sim, "Error al iniciar mutexes globales\n", 0, 1);
}

void	ft_init_forks(t_simul *sim, int n_philos)
{
	int	i;

	i = 0;
	while (i < n_philos)
	{
		if (pthread_mutex_init(&sim->forks[i], NULL) != 0)
			ft_destroy_mutexes(sim, "Error al iniciar mutexes\n", i, 1);
		i++;
	}
}

void	ft_init_philos(t_simul *sim, t_philo *philos, t_mutex *forks, char **av)
{
	int	i;

	i = 0;
	while (i < ft_atoi(av[1]))
	{
		philos[i].philo_id = i + 1;
		philos[i].times.tto_starve = ft_atoi(av[2]);
		philos[i].times.tto_eat = ft_atoi(av[3]);
		philos[i].times.tto_sleep = ft_atoi(av[4]);
		philos[i].meals_required = -1;
		if (av[5])
			philos[i].meals_required = ft_atoi(av[5]);
		philos[i].meals_eaten = 0;
		philos[i].total_philos = ft_atoi(av[1]);
		philos[i].mutexes.left_fork = &forks[i];
		if (i == 0)
			philos[i].mutexes.right_fork = &forks[philos[i].total_philos - 1];
		else
			philos[i].mutexes.right_fork = &forks[i - 1];
		philos[i].mutexes.write_lock = &sim->write_lock;
		philos[i].mutexes.meal_lock = &sim->meal_lock;
		philos[i].mutexes.dead_lock = &sim->dead_lock;
		philos[i].sim_stop_flag = &sim->sim_stop_flag;
		i++;
	}
}
