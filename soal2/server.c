#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h> 

#define PORT 8080
#define MAX_BYTES 1024
#define MAX_ROWS 10
#define SUCCESS_CODE 202
#define FAILED_CODE 404
#define MAX_ROOM 500

struct Account{
    int id;
    int set;
    char username[MAX_BYTES];
    char password[MAX_BYTES];
};

struct Database{
    struct Account rows[MAX_ROWS];
};

struct Connection{
    FILE *file;
    struct Database *db;
};

struct Room{
    int *player1;
    int *player2;
};

void Err(char *message){
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    exit(1);
}

void DatabaseClose(struct Connection *conn){
    if(conn){
        if(conn->file) fclose(conn->file);
        if(conn->db) free(conn->db);
        free(conn);
    }
}

int cfileexists(const char * filename){
    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}

void DatabaseWrite(struct Connection *conn){
    rewind(conn->file);

    int stats = fwrite(conn->db , sizeof(struct Database), 1, conn->file);
    if(stats != 1) Err("failed write database");

    stats = fflush(conn->file);
    if(stats < 0) Err("failed to flush database");

}

struct Connection *OpenDatabase(char *filename){
    struct Connection *conn = malloc(sizeof(struct Connection));

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) Err("failed to get memory");

    int exist = cfileexists(filename);
    if(!exist){
        conn->file = fopen(filename , "w+");
        if(conn->file){
            int i = 0;
            for(i = 0; i < MAX_ROWS; i++) {
                struct Account acc = {.id = i, .set = 0};
                conn->db->rows[i] = acc;
            }

            DatabaseWrite(conn);
        }

    }
    else{
        conn->file = fopen(filename , "r+");
        if(conn->file){
            int stats = fread(conn->db , sizeof(struct Database),1 , conn->file);
            if(stats < 0 ) Err("failed load database");
        } 
    }

    return conn;
}


void Register(struct Connection *conn,const char *username , const char *password){

    int i = 0;
    struct Account *acc;

    for(i=0; i< MAX_ROWS; i++){
        acc = &conn->db->rows[i];
        if(acc->set == 0){
            acc->set = 1;
            char *res = strncpy(acc->username , username , MAX_BYTES);
            if(!res) Err ("Name copy failed!");
            res = strncpy(acc->password, password , MAX_BYTES);
            if(!res) Err ("Password copy failed!");

            DatabaseWrite(conn);

            break;
        }
    }
}

int Login(struct Connection *conn,const char *username , const char *password){
    
    int i = 0;
    struct Account *acc;

    for(i=0; i< MAX_ROWS; i++){
        acc = &conn->db->rows[i];
        if(acc->set == 1 && strcmp(acc->username , username) == 0 && strcmp(acc->password , password) == 0){
            return 1;
        }
    }

    return 0;
}

void AccountList(struct Connection *conn){
  int i = 0;
  struct Database *db = conn->db;
  
  for(i = 0; i < MAX_ROWS; i++) {
    struct Account *cur = &db->rows[i];

    if(cur->set) {
      printf("%d , %s %s\n",
          cur->id, cur->username, cur->password);
    }
  }
}

void GameLoops(int *socket , int *opponentSocket){
    char *atkMsgFromServer = "atkserver";
    while (1)
    {
        char msg[100] = {0};
        char amsg[100] = {0};
        int valread = read( *socket , msg, 1024);
        
        if (strcmp(msg , "attack") == 0){
            send(*opponentSocket , atkMsgFromServer , sizeof(atkMsgFromServer) + 1 , 0 );
        }
        
        else if(strcmp(msg , "end") == 0){
            send(*opponentSocket , "end" , 3 , 0 );
            break;
        }

    }
}

struct Room room[MAX_ROOM];
void Matchmaking(int *socket){
    int i =0;
    int myroom = -1;
    int *opponentSocket;
    int myplayer = -1;
    for ( i = 0; i < MAX_ROOM; i++)
    {
        if(!room[i].player1){
            myroom = i;
            myplayer = 1;
            room[i].player1 = socket;
            break;
        }else if(!room[i].player2){
            myroom = i;
            myplayer = 2;
            room[i].player2 = socket;
            break;
        }
    }    

    while(!room[myroom].player1 || !room[myroom].player2){

    }
    char *code = "202";
    send(*socket , code , strlen(code) , 0 );

    if(myplayer == 1){
        opponentSocket = room[myroom].player2;
    }else{
        opponentSocket = room[myroom].player1;
    }
    GameLoops(socket, opponentSocket);
}

void AfterLogin(int *socket){
    while(1){
        char findOrLogout[100] = {0};
        int valread = read( *socket , findOrLogout, 1024);

        if(strcmp(findOrLogout, "logout") == 0){
            printf("Logout Success\n");
            break;
        }else if(strcmp(findOrLogout, "find") == 0){
            Matchmaking(socket);
        }
    }
}

void *handle_connection(void *socket_arg){
    
    int socket = *(int*)socket_arg;

    while(1){
        
        char buffer[1024] = {0};
        int valread = read( socket , buffer, 1024);        
        
        if (strcmp(buffer, "register") == 0){
            struct Connection *conn = OpenDatabase("tes.txt");

            char username[100] = {0};
            valread = read( socket , username, 1024);
            char password[100] = {0};
            valread = read( socket , password, 1024);
            Register(conn , username , password);
            AccountList(conn);

            DatabaseClose(conn);   

        }else if(strcmp(buffer, "login") == 0){
            struct Connection *conn = OpenDatabase("tes.txt");

            char username[100] = {0};
            valread = read( socket , username, 1024);
            char password[100] = {0};
            valread = read( socket , password, 1024);
            int logStat = Login(conn , username , password);
            if(logStat) {
                printf("Auth Success \n");
                int code = SUCCESS_CODE;
                send(socket , &code , sizeof(SUCCESS_CODE) , 0 );
                AfterLogin(&socket);
                
            }
            else {
                printf("Auth Failed \n");
                int code = FAILED_CODE;
                send(socket , &code , sizeof(SUCCESS_CODE) , 0 );
            }
            DatabaseClose(conn);   
        }
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_id;

    while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))){
        
        if( pthread_create( &thread_id , NULL ,  handle_connection , (void*) &new_socket) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        
    }

    if(new_socket < 0){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
}
