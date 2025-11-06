#include <pthread.h>
#include <stdio.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
int listo = 0;

void* worker(void* arg) {
	pthread_mutex_lock(&m);
	while (!listo) pthread_cond_wait(&c, &m);
	pthread_mutex_unlock(&m);
	puts("worker: empiezo");
	return NULL;
}

int main(void) {
	pthread_t th;
	pthread_create(&th, NULL, worker, NULL);

	puts("main: hilo creado");       // esto saldrá antes
	pthread_mutex_lock(&m);
	listo = 1;
	pthread_cond_signal(&c);         // libera al worker
	pthread_mutex_unlock(&m);

	pthread_join(th, NULL);
	puts("main: join completado");
	return 0;
}