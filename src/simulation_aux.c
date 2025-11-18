/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_aux.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 13:43:18 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/17 13:43:19 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/simulation_aux.h"
#include "../header/aux.h"

void	ft_join_threads(pthread_t monitor_id, t_simul *sim)
{
	int	i;

	i = 0;
	if (pthread_join(monitor_id, NULL) != 0)
		ft_destroy_mutexes(sim, "Error al hacer join en la simulacion\n",
			sim->philos->total_philos, 1);
	i = 0;
	while (i < sim->philos->total_philos)
	{
		if (pthread_join(sim->philos[i].thread_id, NULL) != 0)
			ft_destroy_mutexes(sim, "Error en join los philos\n",
				sim->philos->total_philos, 1);
		i++;
	}
}

void	ft_thinking_time(t_philo *philo) // falta personalizar
{
	size_t	margin;
		// ✅ Thinking time adaptativo para prevenir starvation
	if (philo->total_philos % 2 != 0)
	{
		// Para números impares: thinking time = tto_eat / 2
		ft_usleep(philo->times.tto_eat / 2);
	}
	else
	{
		// Para números pares con margen ajustado: thinking time mínimo
		margin = philo->times.tto_starve
			- (philo->times.tto_eat + philo->times.tto_sleep);
		if (margin < 50) // Si margen < 50ms, añadir thinking time
			usleep(1000);// 1ms para reducir competencia
	}
}

void	ft_eat_sleep_think(t_philo *philo, t_mutex *st_fork, t_mutex *nd_fork)
{
	pthread_mutex_lock(st_fork);
	ft_print_status(philo, "has taken a fork");
	pthread_mutex_lock(nd_fork);
	ft_print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->mutexes.meal_lock);
	philo->times.last_meal = ft_get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->mutexes.meal_lock);
	ft_print_status(philo, "is eating");
	ft_usleep(philo->times.tto_eat);
	pthread_mutex_unlock(nd_fork);
	pthread_mutex_unlock(st_fork);
	ft_print_status(philo, "is sleeping");
	ft_usleep(philo->times.tto_sleep);
	ft_print_status(philo, "is thinking");
	ft_thinking_time(philo);
}

t_bool	ft_starving_check(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos->total_philos)
	{
		pthread_mutex_lock(philos[i].mutexes.meal_lock);
		if (ft_get_time() - philos[i].times.last_meal
			> philos[i].times.tto_starve)
		{
			pthread_mutex_unlock(philos[i].mutexes.meal_lock);
			ft_print_status(&philos[i], "died");
			pthread_mutex_lock(philos->mutexes.dead_lock);
			*philos->sim_stop_flag = true;
			pthread_mutex_unlock(philos->mutexes.dead_lock);
			return (true);
		}
		pthread_mutex_unlock(philos[i].mutexes.meal_lock);
		i++;
	}
	return (false);
}
