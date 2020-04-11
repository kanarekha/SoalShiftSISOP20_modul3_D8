#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

struct args_t{
    char filename[1000];
    char ext[1000];
    int index;
};

pthread_t tids[500];

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

void* threadfunc(void* arg){
    struct args_t *args = arg;
    char dest[1500];
    strcpy(dest, args->ext);
    strcat(dest, "/");
    strcat(dest, args->filename);
    DIR* directory = opendir(args->ext);
    if(directory){
        closedir(directory);
    } else if(ENOENT == errno) {
        mkdir(args->ext, 0777);
        closedir(directory);
    } else{
        printf("fatal error.\n");
        return NULL;
    }
    rename(args->filename, dest);
    free(args);
}

int main(int argc, char** argv){
    int i =0;
    int j=0;
    if(!strcmp(argv[1], "-f")){
        for(int i=2; i<argc; i++){
            char ext[50];
            strcpy(ext,get_filename_ext(argv[i]));
	    if(!strcmp(ext, "")){
                strcpy(ext, "unknown");
            }
	    else if(!strcmp(ext, "*")){
		strcpy(ext, "\*");
	    }
            struct args_t* newarg = malloc(sizeof(struct args_t));
            char *fullpath[1500];
            for(j=0; ext[j]; j++){
                ext[j] = tolower(ext[j]);
            }
            strcpy(newarg->ext, ext);
            strcpy(newarg->filename, argv[i]);
            pthread_create(&tids[i-2], NULL, threadfunc, newarg);
        }
        for(i=0; tids[i]; i++){
            pthread_join(tids[i], NULL);
        }

    } else {
        DIR* directory;
        if (!strcmp(argv[1], "-d")){
            directory = opendir(argv[2]);
	    chdir(argv[2]);
        } else if(!strcmp(argv[1], "*")) {
            char cwd[1500];
            getcwd(cwd, sizeof(cwd));
            directory = opendir(cwd);
        } else {
            printf("Error.");
            return 0;
        }
        if(directory==NULL) {
            printf("Error, directory not found.\n");
            return 0;
        }
        struct dirent * dirent;
        int tid = 0;
        while((dirent = readdir(directory))!=NULL){
            if(!strcmp(dirent->d_name,"..")||!strcmp(dirent->d_name,".")){
                continue;
            }
            if(dirent->d_type == 4) continue;
            if(!strcmp(dirent->d_name,"kategori")) continue; //Nama program
            char ext[100];
            strcpy(ext, get_filename_ext(dirent->d_name));
            if(!strcmp(ext, "")){
                strcpy(ext, "unknown");
            }
	    else if(!strcmp(ext, "*")){
		strcpy(ext, "\*");
	    }
	    else{
                for(i=0; ext[i]; i++){
                    ext[i] = tolower(ext[i]);
                }
            }
            struct args_t* newarg = malloc(sizeof(struct args_t));
            strcpy(newarg->filename, dirent->d_name);
            strcpy(newarg->ext, ext);
            pthread_create(&tids[tid], NULL, threadfunc, newarg);
            tid++;
        }
        for(i=0; tids[i]; i++){
            pthread_join(tids[i],NULL);
        }
        closedir(directory);
    }
    printf("All task finished, Program will now close... \n");
    return 0;
}
