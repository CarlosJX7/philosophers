#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *saludar(void *arg)
{
	int id = *((int *)arg);
	printf("Hola, soy el hilo %d\n", id);
	return NULL;
}

int main(void)
{
	pthread_t hilos[3];
	int ids[3];
	int i;

	i = 0;
	while (i < 3)
	{
		ids[i] = i;
		printf("pthread_create %d\n", i);
		pthread_create(&hilos[i], NULL, saludar, &ids[i]);
		i++;
	}
	i = 0;

	while (i < 3)
	{
		printf("pthread_join %d\n", i);
		pthread_join(hilos[i], NULL);
		i++;
	}
	return 0;
}