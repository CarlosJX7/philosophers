/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:27:22 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/12 12:27:24 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/utils.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	else
		return (0);
}

int	ft_atoi(char *str)
{
	long	nbr;
	long	sign;
	size_t	i;

	nbr = 0;
	sign = 1;
	i = 0;
	while ((str[i] != '\0') && ft_isspace(str[i]) == 1)
		i++;
	if (str[i] == '-')
		sign = -1;
	if ((str[i] == '-') || (str[i] == '+'))
		i++;
	while ((str[i] != '\0') && ('0' <= str[i]) && (str[i] <= '9'))
	{
		nbr = (nbr * 10) + (str[i] - '0');
		if (nbr > 2147483647 && sign == 1)
			return (-1);
		if (nbr > 2147483648 && sign == -1)
			return (0);
		i++;
	}
	return (sign * nbr);
}

void	ft_one_philo(t_philo *philo)
{
	if (philo->total_philos == 1)
	{
		pthread_mutex_lock(philo->mutexes.left_fork);
		ft_print_status(philo, "has taken a fork");
		// Esperar hasta que el monitor detecte muerte
		while (!ft_check_death_flag(philo))
			ft_usleep(1);
		pthread_mutex_unlock(philo->mutexes.left_fork);
		return ;
	}
	return ;
}

t_bool	ft_check_death_flag(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(philo->mutexes.dead_lock);
	result = *philo->sim_stop_flag;// O philo->sim->simulation_stopped
	pthread_mutex_unlock(philo->mutexes.dead_lock);
	return (result);
}
