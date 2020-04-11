#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>
#define z unsigned long long

struct number{
	z value;
};

void *sumi(void* arg) {
	struct number* angka;
	angka = (struct number *) arg;

	z val = angka->value;
	val = (((val+1)*val)/2);
	
	printf("%llu\t", val);
}

int main(int argc, char* argv[]){
	int counter = 0;
	key_t key = 1500;
	int *tes, curr[20];

	int shmid = shmget(key, 20, IPC_CREAT | 0666);
    tes = (int *)shmat(shmid, 0, 0);
	
	printf("Hasil perkalian matriks soal 4a:\n");
	for(int a=0; a<20; a++) {
		curr[a] = tes[a];
		printf("%d\t", tes[a]);
		if((a%5) == 4) {
			printf("\n");
		}
	}
	shmdt((void *) tes);
	shmctl(shmid, IPC_RMID, NULL);
	
	printf("\nHasil matriks penjumlahan :\n");
	
	pthread_t threads[20];

	for(int i=0; i<20; i++) {
		struct number* angka = (struct number*)malloc(sizeof(struct number));
		angka->value = curr[i];

		pthread_create(&threads[i], NULL, sumi, (void *)angka);
		pthread_join(threads[i], NULL);
		
		if((i%5) == 4) {
			printf("\n");
		}	
	}
	for(int i=0; i<20; i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}