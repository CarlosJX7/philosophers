#include <pthread.h>
#include <stdio.h>

typedef struct s_compartido
{
	int				contador;
	pthread_mutex_t	mutex;
}	t_compartido;

typedef struct s_hilo
{
	int				id;
	t_compartido	*compartido;
}	t_hilo;

void	*incrementar(void *arg)
{
	t_hilo *mi = (t_hilo *)arg;
	int i = 0;

	while (i < 50000)
	{
		pthread_mutex_lock(&mi->compartido->mutex);
		mi->compartido->contador++;
		pthread_mutex_unlock(&mi->compartido->mutex);
		i++;
	}
	return (NULL);
}

int	main(void)
{
	pthread_t hilos[2];
	t_compartido compartido;
	t_hilo hilos_data[2];
	int i;

	compartido.contador = 0;
	pthread_mutex_init(&compartido.mutex, NULL);
	i = 0;
	while (i < 2)
	{
		hilos_data[i].id = i;
		hilos_data[i].compartido = &compartido;
		pthread_create(&hilos[i], NULL, incrementar, &hilos_data[i]);
		i++;
	}
	i = 0;
	while (i < 2)
	{
		pthread_join(hilos[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&compartido.mutex);
	printf("Contador: %d\n", compartido.contador);
	return (0);
}