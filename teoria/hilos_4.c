#include <pthread.h>
#include <stdio.h>

int contador = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void	*incrementar(void *arg)
{
	int i = 0;
	while (i < 100000)
	{
		pthread_mutex_lock(&mutex);
		contador++;
		pthread_mutex_unlock(&mutex);
		i++;
	}
	return (NULL);
}

int	main(void)
{
	pthread_t h1, h2;

	pthread_create(&h1, NULL, incrementar, NULL);
	pthread_create(&h2, NULL, incrementar, NULL);
	pthread_join(h1, NULL); //Hasta que no termina h1, main no continua
	pthread_join(h2, NULL); //Es posible que termine pero hasta que el anterior
							// join no termine, esta linea no se ejecuta
	pthread_mutex_destroy(&mutex);
	printf("Contador: %d\n", contador);
	return (0);
}