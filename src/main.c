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
		error_message("Cantiad de argumentos erronea\n");
	while (i < argc)
	{
		num = ft_atoi(argv[i]);
		if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT)) 
			error_message("Numero de filosofos fuera de rango\n");
		else if (i == 5 && (num < 0) || num > INT_MAX)
			error_message("Numero de comidas fuera de rango\n");
		else if (i > 1 && i < 5 && (num < 1 || num > INT_MAX))
			error_message("Tiempos fuera de rango\n");
		i++;
	}
}


int main(int argc, char *argv)
{
	t_philo philos[PHILO_MAX_COUNT];
	t_mutex forks[PHILO_MAX_COUNT];
	t_simulation table;

	ft_args_check(argc, argv);
	
}