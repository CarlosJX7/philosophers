/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:51:03 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/06 14:51:04 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

#include "../header/aux.h"
#include "../header/philo.h"

int ft_strlen(char *s);
int ft_atoi(char *str);
void ft_one_philo(t_philo *philo);
t_bool ft_check_death_flag(t_philo *philo);
#endif
