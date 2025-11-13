/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:26:35 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/12 12:26:36 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUX_H
# define AXUX_H

#include "../header/philo.h"
#include <sys/time.h>

void ft_print_error(char *message, int exit_code);
void ft_destroy_mutexes(t_simulation *sim, char *message, int n_philos, int exit_code);
size_t ft_get_time(void);
void ft_print_status(t_philo *philo, char *status);

#endif
