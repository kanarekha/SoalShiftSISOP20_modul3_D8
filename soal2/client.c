#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#define PORT 8080

int isPlay = 0;
int matchFound = 0;
int health = 100;
int isWin = 1;

void changemode(int);
int  kbhit(void);

void *DisplayMessage(void *arg){
    char *message = (char*)arg;
    // system("clear");
        
    while (matchFound == 0)
    {
        printf("%s \n", message);
        sleep(1);
    }
    
}

void *RecvAttack(void *arg){
    int socket = *(int*)arg;
    while (matchFound)
    {
        char msg[100] = {0};
        int valread = read( socket , msg, 1024);
        if(strcmp(msg , "atkserver") == 0 && health > 0){
            health -= 10;
            printf("Your Health : %d\n", health);
            if(health <= 0){
                send(socket , "end" , 3 , 0 );
                isWin = 0;
		matchFound=0;
                printf("Game Over You Lose !!! \n");
            }
        }
        else if(strcmp(msg , "end") == 0){
            matchFound = 0;
            printf("Game Over You Win !!! \n");
            break;
        }
    }
}

void Play(int *socket){
    isWin = 1;
    health = 100;
    char input;
    changemode(1);
    char *attack = "attack";

    pthread_t thread_id;
    if( pthread_create( &thread_id , NULL ,  RecvAttack , (void*)socket) < 0)
    {
        perror("could not create thread");
        return;
    }
        
    while(matchFound){
        
        if(!kbhit()){
            
            input = getchar();

            if(health <= 0){
                matchFound = 0;
                break;
            }else{
                if(input == ' ' && matchFound){
                    printf("hit !!\n");
                    send(*socket , attack , strlen(attack) , 0 );
                }else if(matchFound == 0){
                    send(*socket , "end" , 3 , 0 );
                }
            }            
        }
    }
    changemode(0);
}

void WaitingMatchmaking(int *socket){

    char *msg = "Waiting for players ...";
    pthread_t thread_id;
    if( pthread_create( &thread_id , NULL , DisplayMessage , (void*)msg) < 0)
    {
        perror("could not create thread");
        return;
    }

    char code[100] = {0};
    int valread = read( *socket , code, 1024);
    
    if(strcmp(code , "202") == 0){
        matchFound = 1;
        printf("Game dimulai \n");
        Play(socket);
    }

    matchFound = 0;
}

void *GameLoops(void *socket_arg){
    int socket = *(int*)socket_arg;
    while(1){
        char input[100];
        
                
        printf("1. Find Match\n2. Logout\nChoice : ");
        scanf("%s", input);
        send(socket , input , strlen(input) , 0 );
        if(strcmp(input , "logout") == 0){
            isPlay = 0;
            break;
        }else if(strcmp(input , "find") == 0){
            WaitingMatchmaking(&socket);
        }
        
        memset(input , 0 , sizeof(input));
    }
}
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while(1){
        if(isPlay == 0){
            printf("1. Register\n2. Login\nChoices : ");
            char choice[256];
            scanf("%s", choice);    
            send(sock , choice , strlen(choice) , 0 );

            if (strcmp(choice, "register") == 0){
                
                char input[100];
                
                printf("Username : ");
                scanf("%s", input);
                getchar();
                
                send(sock , input , strlen(input) , 0 );

                memset(input , 0 , sizeof(input));
                printf("Password : ");
                gets(input);
                send(sock , input , strlen(input) , 0 );

                printf("Register success\n");
                
            }
            if (strcmp(choice, "login") == 0){
                
                char input[100];
                
                printf("Username : ");
                scanf("%s", input);
                getchar();
                
                send(sock , input , strlen(input) , 0 );

                memset(input , 0 , sizeof(input));
                printf("Password : ");
                gets(input);
                send(sock , input , strlen(input) , 0 );

                int code = -1;
                valread = recv(sock, &code, sizeof(code), 0);
                if (code == 202){
                    printf("Login Success\n");
                    isPlay = 1;
                    
                    pthread_t thread_id;
                    if( pthread_create( &thread_id , NULL ,  GameLoops , (void*) &sock) < 0)
                    {
                        perror("could not create thread");
                        return 1;
                    }

                }else{
                    printf("Login Failed\n");
                }
                
            }
            memset(choice , 0 , sizeof(choice));
                
        }

    }

    return 0;
}


void changemode(int dir){
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
 
int kbhit (void){
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

