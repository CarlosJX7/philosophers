#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

long	obtener_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	*tarea(void *arg)
{
	int id = *(int *)arg;
	long inicio = obtener_ms();
	usleep(100000);
	long fin = obtener_ms();
	printf("Hilo %d tardó %ld ms\n", id, fin - inicio);
	return (NULL);
}

int	main(void)
{
	pthread_t hilos[3];
	int ids[3] = {0, 1, 2};
	int i = 0;

	while (i < 3)
	{
		pthread_create(&hilos[i], NULL, tarea, &ids[i]);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		pthread_join(hilos[i], NULL);
		i++;
	}
	return (0);
}