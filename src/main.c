/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:35:10 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/06 14:35:11 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <limits.h>
#include "../header/philo.h"
#include "../header/initialization.h"
#include "../header/utils.h"
#include "../header/philo_simulation.h"


//1. number_of_philosophers and forks
//2. time_to_die (ms)
//3. time_to_eat (ms)
//4. time_to_sleep (ms)
//5. [number_of_times_each_philosopher_must_eat]
void ft_args_check(int argc, char **argv)
{
	int  i;
	long num;

	i = 0;
	if (argc < 5 || argc > 6)
	ft_print_error("Error en la cantidad de args\n", 1);
	while (i < argc)
	{
		num = ft_atoi(argv[i]);
		if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT)) 
			ft_print_error("Cantidad de philos fuera de rango\n", 1);
		else if (i == 5 && (num < 0 || num > INT_MAX))
			ft_print_error("Cantidad de comidas fuera de rango\n", 1);
		else if (i > 1 && i < 5 && (num < 1 || num > INT_MAX))
			ft_print_error("Tiempo fuera de rango\n", 1);
		i++;
	}
}


int main(int argc, char **argv)
{
	t_philo philos[PHILO_MAX_COUNT];
	t_mutex forks[PHILO_MAX_COUNT];
	t_simulation sim;

	ft_args_check(argc, argv);
	ft_init_simulation(&sim, philos, forks);
	ft_init_forks(&sim, ft_atoi(argv[1]));
	ft_init_philosophers(&sim, philos, forks, argv);
	ft_init_philosophers(&sim, philos, forks, argv);
	ft_start_simulation_threads(&sim);
	ft_destroy_mutexes(&sim, NULL, philos->total_philos, 0);
}