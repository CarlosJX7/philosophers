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
	int		i;

	philos = (t_philo *)ptr;
	i = 0;
	while (true)
	{
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
				return (NULL);
			}
			pthread_mutex_unlock(philos[i].mutexes.meal_lock);
			i++;
		}
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

void    ft_philos_routine(t_philo *philo)
{
    t_mutex *first_fork;
    t_mutex *second_fork;

    if (philo->total_philos == 1)
    {
        ft_one_philo(philo);
        return ;
    }
    
    // ✅ Ordenar por dirección de memoria para evitar deadlock
    // Esta estrategia es más robusta que par/impar
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
    
    pthread_mutex_lock(first_fork);
    ft_print_status(philo, "has taken a fork");
    pthread_mutex_lock(second_fork);
    ft_print_status(philo, "has taken a fork");
    
    pthread_mutex_lock(philo->mutexes.meal_lock);
    philo->times.last_meal = ft_get_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(philo->mutexes.meal_lock);
    
    ft_print_status(philo, "is eating");
    ft_usleep(philo->times.tto_eat);
    
    pthread_mutex_unlock(second_fork);
    pthread_mutex_unlock(first_fork);
    
    ft_print_status(philo, "is sleeping");
    ft_usleep(philo->times.tto_sleep);
    ft_print_status(philo, "is thinking");
    
    // ✅ Thinking time adaptativo para prevenir starvation
    if (philo->total_philos % 2 != 0)
    {
        // Para números impares: thinking time = tto_eat / 2
        ft_usleep(philo->times.tto_eat / 2);
    }
    else
    {
        // Para números pares con margen ajustado: thinking time mínimo
        size_t margin = philo->times.tto_starve - 
                       (philo->times.tto_eat + philo->times.tto_sleep);
        if (margin < 50)  // Si margen < 50ms, añadir thinking time
            usleep(1000);  // 1ms para reducir competencia
    }
}


void	*ft_start_philo_thread(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	/* CAMBIO 4: Eliminado el busy-wait (while birth_time == 0)
	 * Ya no es necesario porque timestamps se inicializan ANTES
	 * de crear los threads (ver ft_start_simulation_threads) */
	if (philo->philo_id % 2 == 0)
		ft_usleep(1);
	while (!ft_check_death_flag(philo))
		ft_philos_routine(philo);
	return (NULL);
}

void	ft_start_simulation_threads(t_simulation *sim)
{
	pthread_t	monitor_id;
	int			i;
	size_t		start_time;

	/* CAMBIO 5: Inicializar timestamps ANTES de crear threads
	 * Esto previene race conditions y elimina necesidad de busy-wait
	 * Todos los philos empiezan con el mismo timestamp inicial */
	start_time = ft_get_time();
	i = 0;
	while (i < sim->philos->total_philos)
	{
		sim->philos[i].times.birth_time = start_time;
		sim->philos[i].times.last_meal = start_time;
		i++;
	}
	/* CAMBIO 6: Crear threads de filósofos DESPUÉS de inicializar timestamps
	 * Orden correcto: init timestamps → crear philos → crear monitor */
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
	if (pthread_join(monitor_id, NULL) != 0)
		ft_destroy_mutexes(sim, "Error al hacer join en la simulacion\n",
			sim->philos->total_philos, 1);
	/* CAMBIO 7: pthread_join en lugar de pthread_detach
	 * join ESPERA a que los threads terminen antes de continuar
	 * detach solo los marca como independientes pero no espera
	 * Necesitamos join para terminación controlada con el flag */
	i = 0;
	while (i < sim->philos->total_philos)
	{
		if (pthread_join(sim->philos[i].thread_id, NULL) != 0)
			ft_destroy_mutexes(sim, "Error en join los philos\n",
				sim->philos->total_philos, 1);
		i++;
	}
}
