#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>


int matriksA[4][2], matriksB[2][5] , matriksC[4][5];

void* multi(void* arg) 
{ 
    
    int i,j,k,sum=0;

     for (i = 0; i < 4; i++) {//udah
      for (j = 0; j < 5; j++) {
        for (k = 0; k < 2; k++) {
          sum = sum + matriksA[i][k]*matriksB[k][j];
        }
 
        matriksC[i][j] = sum;
        sum = 0;
      }
    }
 
                
} 
int main(){

    pthread_t tid[3]; 
    key_t key = 1500;
	int *tes, a = 0;
	int shmid = shmget(key, 20, IPC_CREAT | 0666);

	tes = (int *)shmat(shmid, 0, 0);
        
    int i,j ;
    int val=1;

    //matriks A
    printf("MATRIKS A\n");
    for(i=0;i<4;i++){
        for(j=0;j<2;j++){
            matriksA[i][j]=val;
            printf("%d  ",val);
        //    printf("matriksA[%d][%d] : %d  ",i,j,val);
            val++;            
        }
        printf("\n");
    }
    printf("\n");
    printf("MATRIKS B\n");
    for(i=0;i<2;i++){
        for(j=0;j<5;j++){
            matriksB[i][j]=val;
            printf("%d  ",val);
          //  printf("matriksB[%d][%d] : %d  ",i,j,val);
            val++;            
        }
        printf("\n");
    }

    for (int i = 0; i < 2; i++) { //create thread
        int* p; 
        pthread_create(&tid[i], NULL, multi, (void*)(p)); 
    } 

    for (int i = 0; i < 2; i++) { //join
        pthread_join(tid[i], NULL);   
    }  

    // Displaying the result matrix 
    printf("\nHasilnya :\n");
         
    for (int i = 0; i < 4; i++) { 
        for (int j = 0; j < 5; j++) { 
            tes[a]=matriksC[i][j];
            a++;
            printf("%d " , matriksC[i][j]);        
        
        }
        printf("\n");
    }
    shmdt((void *) tes); 
return 0;
}