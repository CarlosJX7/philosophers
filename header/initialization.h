/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 15:40:14 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/06 15:40:18 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INITIALIZATION_H
# define INITIALIZATION_H

#include "philo.h"

void ft_init_simulation(t_simulation *sim, t_philo *philos, t_mutex *forks);
void ft_init_forks(t_simulation *sim, int n_philos);
void ft_init_philosophers(t_simulation *sim, t_philo *philos, t_mutex *forks, char **argv);


# endif