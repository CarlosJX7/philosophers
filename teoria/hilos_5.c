#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int dato_compartido = 0;

/*
void	*trabajador(void *arg)
{
	int id = *(int *)arg;
	printf("Hilo %d: esperando acceso...\n", id);
	pthread_mutex_lock(&mutex);
	printf("Hilo %d: acceso concedido, dato=%d\n", id, dato_compartido);
	dato_compartido += 10;
	pthread_mutex_unlock(&mutex);
	return (NULL);
}
*/

void* trabajador(void* arg) {
	int id = *(int*)arg;
	printf("Hilo %d: esperando acceso...\n", id);

	int antes, despues;
	pthread_mutex_lock(&mutex);
	antes = dato_compartido;
	dato_compartido += 10;
	despues = dato_compartido;
	pthread_mutex_unlock(&mutex);

	printf("Hilo %d: acceso concedido, dato pasó de %d a %d\n", id, antes, despues);
	return NULL;
}


int	main(void)
{
	pthread_t hilos[3];
	int ids[3] = {1, 2, 3};
	int i;

	pthread_mutex_lock(&mutex);
	i = 0;
	while (i < 3)
	{
		pthread_create(&hilos[i], NULL, trabajador, &ids[i]);
		i++;
	}
	printf("Main: hilos creados, liberando en 2 seg...\n");
	sleep(2);
	pthread_mutex_unlock(&mutex);
	i = 0;
	while (i < 3)
	{
		pthread_join(hilos[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	printf("Dato final: %d\n", dato_compartido);
	return (0);
}