#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N          5 //numero de filosofos
#define LEFT       (i + N-1) % N // numero de filosofos a esquerda
#define RIGHT      (i + 1) % N //numero de filosofos a direita
#define THINKING   0 //filosofo esta pensando
#define HUNGRY     1 //filosofo esta tentando pegar o garfo
#define EATING     2 //filosofo esta comendo

typedef sem_t semaphore; //tipo semaforo
int up(sem_t *mutex){
return sem_post(mutex); //função sem_post desbloqueia o semaforo
}

int down(int *mutex){
return sem_wait(mutplex); //função sem_wait bloqueia o semaforo
}

//assinaturas das funções
void take_fork(int i); //pega o garfo para o filosofo
void put_fork(int i); //devolve os garfos
void test(int i); //tenta pegar os dois garfos
void* philosopher(void* num); //função do filosofo

int phil[N] = { 0, 1, 2, 3, 4 }; //array dos filosofos
int state[N]; //array de estados
semaphore mutex;
semaphore S[N];

void* philosopher(void* num) {

	while (1) {
		int* i = num;
		sleep(1);
		take_fork(*i);
		sleep(0);
		put_fork(*i);
	}
}

void take_fork(int i) {

	down(&mutex);
	state[i] = HUNGRY;
	printf("Filósofo %d está com fome\n", i + 1);
	test(i); 
	up(&mutex);
	down(&S[i]); 
	sleep(1);
}

void put_fork(int i) {

	down(&mutex);
	state[i] = THINKING;
	printf("Filósofo %d colocando garfo %d e %d na mesa\n", i + 1, LEFT + 1, i + 1);
	printf("Filósofo %d está pensando\n", i + 1);
	test(LEFT);
	test(RIGHT);
	up(&mutex);
}

void test(int i) {

	if (state[i] == HUNGRY && state[LEFT] != EATING	&& state[RIGHT] != EATING) {

		state[i] = EATING;
		sleep(2);
		printf("Filósofo %d pegou o garfo %d e %d\n",	i + 1, LEFT + 1, i + 1);
		printf("Filósofo %d está comendo\n", i + 1);
		up(&S[i]);
	}
}

int main() {

	int i;
	pthread_t thread_id[N];
	sem_init(&mutex, 0, 1);

	for (i = 0; i < N; i++) {
	sem_init(&S[i], 0, 0);
	}
	
	for (i = 0; i < N; i++) {
		pthread_create(&thread_id[i], NULL,	philosopher, &phil[i]);
		printf("Filósofo %d está pensando\n", i + 1);
	}

	for (i = 0; i < N; i++){
	pthread_join(thread_id[i], NULL);
	}
}
