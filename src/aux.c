#include "../header/aux.h"

void ft_print_error(char *message, int exit_code)
{
	if (message)
		write(2, message, ft_strlen(message));
	exit(exit_code);
}

void ft_destroy_mutexes(t_simulation *sim, char *message, int n_philos, int exit_code)
{
	while (n_philos > 0) //bucle para destruir los mutexes de los tenedores
	{
		pthread_mutex_destroy(&sim->forks[n_philos - 1]);
		n_philos--;
	}
	pthread_mutex_destroy(&sim->meal_lock);
	pthread_mutex_destroy(&sim->write_lock);
	ft_print_error(message, exit_code);
}