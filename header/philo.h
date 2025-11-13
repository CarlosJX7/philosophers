/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cinaquiz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:19:04 by cinaquiz          #+#    #+#             */
/*   Updated: 2025/11/06 14:19:06 by cinaquiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>

# define PHILO_MAX_COUNT 200

typedef pthread_t		t_id;
typedef pthread_mutex_t	t_mutex;
typedef struct timeval	t_timeval;

// Estructura que agrupa los mutexes asociados a un filósofo individual.
// Los mutexes controlan el acceso a recursos compartidos como tenedores y operaciones de escritura/comida,
// previniendo race conditions en la simulación concurrente.
typedef struct s_mutexs
{
    t_mutex *left_fork;  // Puntero al mutex del tenedor izquierdo para sincronizar su acceso.
    t_mutex *right_fork; // Puntero al mutex del tenedor derecho.
    t_mutex *write_lock; // Puntero al mutex global para sincronizar la escritura en consola.
    t_mutex *meal_lock;  // Puntero al mutex para proteger el conteo de comidas.
} t_mutexes;

// Estructura que almacena los tiempos relacionados con el ciclo de vida de un filósofo.
// Incluye tiempos de espera, eventos y timestamps para gestionar el estado temporal y decidir acciones.
typedef struct s_times
{
    size_t tto_starve;       // Tiempo máximo sin comer antes de morir (en milisegundos).
    size_t tto_eat;       // Duración de una sesión de comida.
    size_t tto_sleep;     // Duración de una sesión de sueño.
    size_t last_meal;    // Timestamp de la última comida para calcular riesgos.
    size_t birth_time;        // Timestamp de nacimiento del filósofo para tiempos relativos.
}	t_times;

// Estructura que representa a un filósofo individual en la simulación.
// Contiene su estado, tiempos, mutexes y estadísticas, permitiendo que cada hilo maneje su propio filósofo.
typedef struct s_philo
{
    int	philo_id;        // Identificador único del filósofo (1 a PHILO_MAX_COUNT).
    t_times	times;               // Estructura con los tiempos del filósofo.
    t_mutexes mutexes;          // Estructura con los mutexes asociados.
    int meals_required;         // Número de comidas que debe consumir antes de terminar (opcional).
    t_id thread_id;             // ID del hilo que ejecuta este filósofo.
    int meals_eaten;            // Contador de comidas consumidas.
    int total_philos;     // Número total de filósofos en la simulación.
}	t_philo;

// Estructura central que coordina la simulación completa.
// Contiene arreglos de filósofos y tenedores, además de mutexes globales para sincronización.
typedef struct s_simulation
{
    t_mutex *forks;             // Arreglo de mutexes representando los tenedores (uno por filósofo).
    t_philo *philos;      // Arreglo de estructuras t_philo (uno por filósofo).
    t_mutex meal_lock;          // Mutex global para proteger operaciones de comidas.
    t_mutex write_lock;         // Mutex global para sincronizar la salida a consola.
}	t_simulation;

#endif