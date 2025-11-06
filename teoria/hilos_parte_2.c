#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_datos
{
	int		id;
	char	*mensaje;
}	t_datos;

void	*mostrar(void *arg)
{
	t_datos *datos = (t_datos *)arg;
	printf("Hilo %d: %s\n", datos->id, datos->mensaje);
	return (NULL);
}

int	main(void)
{
	pthread_t hilos[2];
	t_datos datos[2];

	datos[0].id = 0;
	datos[0].mensaje = "Primer hilo";
	datos[1].id = 1;
	datos[1].mensaje = "Segundo hilo";
	pthread_create(&hilos[0], NULL, mostrar, &datos[0]);
	pthread_create(&hilos[1], NULL, mostrar, &datos[1]);
	pthread_join(hilos[0], NULL);
	pthread_join(hilos[1], NULL);
	return (0);
}