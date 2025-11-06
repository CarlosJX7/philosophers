#include <pthread.h>
#include <stdio.h>

int contador = 0; // Contador por fuera para que ambos hilos accedan al mismo recuerso

void	*incrementar(void *arg)
{
	int i = 0;
	while (i < 100000)
	{
		contador++;
		i++;
	}
	return (NULL);
}

int	main(void)
{
	pthread_t h1, h2;

	pthread_create(&h1, NULL, incrementar, NULL);
	pthread_create(&h2, NULL, incrementar, NULL);
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);
	printf("Contador: %d (esperado: 200000)\n", contador);
	return (0);
}