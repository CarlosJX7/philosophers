/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_aux.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:47:52 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/17 12:47:54 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void ft_join_threads(pthread_t monitor_id, t_simul *sim);
void ft_thinking_time(t_philo *philo);
void ft_eat_sleep_think(t_philo *philo, t_mutex *first_fork, t_mutex *second_fork);
t_bool ft_starving_check(t_philo *philos);